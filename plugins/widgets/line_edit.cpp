#include "line_edit.h"

#include "gui/anchor.h"

SplinePoint::SplinePoint(Vec2d position, Vec2d frame_size)
    : Widget(Anchor(position, Vec2d(10.0, 10.0), Vec2d(0.0, 0.0),
                    Vec2d(0.0, 0.0))),
      frame_size_(frame_size) {}

static Vec2d clamp(const Vec2d& vector, const Vec2d& boundary) {
    Vec2d vec = vector;
    if (vec.x > boundary.x / 2.0) vec.x = boundary.x / 2.0;
    if (vec.x < -boundary.x / 2.0) vec.x = -boundary.x / 2.0;
    if (vec.y > boundary.y / 2.0) vec.y = boundary.y / 2.0;
    if (vec.y < -boundary.y / 2.0) vec.y = -boundary.y / 2.0;
    return vec;
}

void SplinePoint::onMouseMove(const plug::MouseMoveEvent& event,
                              plug::EHC& context) {
    if (follows_mouse_) {
        Vec2d delta = context.stack.restore(event.pos) -
                      context.stack.restore(mouse_pos_);

        if (fixed_) delta *= Vec2d(0.0, 1.0);

        Vec2d new_pos =
            clamp(getLayoutBox().getPosition() + delta, frame_size_);

        getLayoutBox().setPosition(new_pos);
    }

    mouse_pos_ = event.pos;
}

void SplinePoint::onMousePressed(const plug::MousePressedEvent& event,
                                 plug::EHC& context) {
    if (!covers(context.stack, event.pos)) return;

    if (event.button_id == plug::MouseButton::Left) {
        follows_mouse_ = true;
        mouse_pos_ = event.pos;
    }

    if (event.button_id == plug::MouseButton::Right && !fixed_) {
        alive_ = false;
    }

    context.stopped = true;
}

void SplinePoint::onMouseReleased(const plug::MouseReleasedEvent& event,
                                  plug::EHC& context) {
    if (event.button_id == plug::MouseButton::Left) follows_mouse_ = false;
}

static const double CMP_EPS = 1e-7;

static double interp(double alpha, double beta, double t_a, double t_b,
                     double time) {
    return (alpha * (t_b - time) + beta * (time - t_a)) / (t_b - t_a);
}

static Vec2d interpolate(Vec2d points[4], double time) {
    double approx_a0 =
        interp(points[0].y, points[1].y, points[0].x, points[1].x, time);
    double approx_a1 =
        interp(points[1].y, points[2].y, points[1].x, points[2].x, time);
    double approx_a2 =
        interp(points[2].y, points[3].y, points[2].x, points[3].x, time);

    double approx_b0 =
        interp(approx_a0, approx_a1, points[0].x, points[2].x, time);
    double approx_b1 =
        interp(approx_a1, approx_a2, points[1].x, points[3].x, time);

    double approx_c0 =
        interp(approx_b0, approx_b1, points[0].x, points[3].x, time);

    return Vec2d(time, approx_c0);
}

const unsigned INTERP_STEPS = 32;

LineEdit::LineEdit(const plug::LayoutBox& box) : Widget(box) {
    Vec2d size = getLayoutBox().getSize();

    points_.push(SplinePoint(size * -0.5, size));
    points_.push(SplinePoint(size * 0.5, size));
    points_[0].setFixed(true);
    points_[1].setFixed(true);
}

void LineEdit::draw(plug::TransformStack& stack, plug::RenderTarget& target) {
    Widget::draw(stack, target);

    sortPoints();

    size_t vertex_count = (points_.size() - 1) * INTERP_STEPS + 1;

    plug::VertexArray array(plug::PrimitiveType::LinesStrip, vertex_count);

    array[0].position = getLayoutBox().getSize() * -0.5;
    array[array.getSize() - 1].position = getLayoutBox().getSize() * 0.5;

    stack.enter(getLocalCoords());

    for (size_t id = 0; id < points_.size(); ++id) {
        points_[id].draw(stack, target);
    }

    for (size_t id = 0; id < points_.size(); ++id) {
        array[id * INTERP_STEPS].position = points_[id].getPos();
    }

    for (size_t id = 0; id + 1 < points_.size(); ++id) {
        static Vec2d dots[4] = {Vec2d(0.0, 0.0), Vec2d(0.0, 0.0),
                                Vec2d(0.0, 0.0), Vec2d(0.0, 0.0)};

        dots[1] = points_[id].getPos();
        dots[2] = points_[id + 1].getPos();
        dots[0] =
            (id == 0) ? dots[1] * 2.0 - dots[2] : points_[id - 1].getPos();
        dots[3] = (id + 2 >= points_.size())
                      ? dots[2] * 2.0 - dots[1]
                      : array[(id + 2) * INTERP_STEPS].position;

        for (unsigned int_id = 1; int_id < INTERP_STEPS; ++int_id) {
            double param = (double)int_id / INTERP_STEPS;
            double time = dots[1].x * (1.0 - param) + dots[2].x * param;
            array[id * INTERP_STEPS + int_id].position =
                interpolate(dots, time);
        }
    }

    for (size_t id = 0; id < array.getSize(); ++id) {
        array[id].position =
            clamp(array[id].position, getLayoutBox().getSize());
        array[id].position = stack.apply(array[id].position);
    }

    stack.leave();

    target.draw(array);
}

void LineEdit::onEvent(const plug::Event& event, plug::EHC& context) {
    context.stack.enter(getLocalCoords());

    for (size_t id = 0; id < points_.size(); ++id) {
        points_[id].onEvent(event, context);

        if (context.stopped == true) break;
    }

    context.stack.leave();

    sortPoints();

    Widget::onEvent(event, context);
}

void LineEdit::onMousePressed(const plug::MousePressedEvent& event,
                              plug::EHC& context) {
    if (!covers(context.stack, event.pos)) return;
    if (context.stopped) return;

    if (event.button_id == plug::MouseButton::Right && points_.size() < 15) {
        Vec2d pos = context.stack.restore(event.pos);
        points_.push(SplinePoint(pos, getLayoutBox().getSize()));
    }

    context.stopped = true;
}

double LineEdit::getAt(double input) const {
    input = (input - 0.5) * getLayoutBox().getSize().x;

    size_t index = 0;

    for (size_t id = 0; id < points_.size(); ++id) {
        if (points_[id].getPos().x < input) index = id;
    }

    static Vec2d dots[4] = {Vec2d(0.0, 0.0), Vec2d(0.0, 0.0), Vec2d(0.0, 0.0),
                            Vec2d(0.0, 0.0)};

    dots[1] = points_[index].getPos();
    dots[2] = points_[index + 1].getPos();
    dots[0] =
        (index == 0) ? dots[1] * 2.0 - dots[2] : points_[index - 1].getPos();
    dots[3] = (index + 2 >= points_.size()) ? dots[2] * 2.0 - dots[1]
                                            : points_[index + 2].getPos();

    double interp = interpolate(dots, input).y;

    interp = interp / getLayoutBox().getSize().y + 0.5;

    return interp;
}

unsigned char LineEdit::getValue(unsigned char value) const {
    double source = (double)value / 255.0;
    double destination = getAt(source);

    if (destination < 0.0) destination = 0.0;
    if (destination > 1.0) destination = 1.0;

    return (unsigned char)(destination * 255.0);
}

void LineEdit::sortPoints() {
    for (size_t id = points_.size(); id > 0; --id) {
        if (points_[id - 1].getAlive()) continue;

        if (id != points_.size()) points_[id - 1] = points_[points_.size() - 1];
        points_.pop();
    }

    for (size_t rep = 0; rep + 1 < points_.size(); ++rep) {
        for (size_t id = 0; id + 1 < points_.size(); ++id) {
            size_t id_x = id;
            size_t id_y = id + 1;

            if (points_[id_x].getPos().x < points_[id_y].getPos().x) continue;

            SplinePoint point = points_[id_x];

            points_[id_x] = points_[id_y];
            points_[id_y] = point;
        }
    }
}
