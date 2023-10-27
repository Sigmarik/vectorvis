#include "tool.h"

#include <SFML/Graphics.hpp>

#include "src/graphics/renderable/sf_cheatsheet.hpp"

static const sf::Color PREVIEW_COLOR = sf::Color(125, 255, 125);

static sf::Color get_fg_color(const ImageView& view) {
    ToolPalette* pallet = view.get_pallet();

    if (pallet == nullptr) return sf::Color(255, 0, 255);

    return pallet->get_fg_color();
}

static sf::Color get_bg_color(const ImageView& view) {
    ToolPalette* pallet = view.get_pallet();

    if (pallet == nullptr) return sf::Color(255, 125, 255);

    return pallet->get_bg_color();
}

void LineTool::render(const MatrixStack<Mat33d>& stack,
                      sf::RenderTarget& target, const ImageView& image_view) {
    static sf::VertexArray line(sf::Lines, 2);

    if (!active_) return;

    Vec3d begin = stack.top() * extrude(image_view.to_scene_coords(start_pos_));
    Vec3d end = stack.top() * extrude(image_view.to_scene_coords(end_pos_));

    line[0].position = to_Vector2f(begin);
    line[1].position = to_Vector2f(end);

    line[0].color = line[1].color = PREVIEW_COLOR;

    target.draw(line);
}

void LineTool::on_main(ButtonState state, Vec2d pos, ImageView& view) {
    if (state == BTN_DOWN) {
        start_pos_ = pos;
        end_pos_ = pos;
        active_ = true;
    } else {
        confirm(view);
    }
}

void LineTool::on_move(Vec2d pos, ImageView& view) { end_pos_ = pos; }

static void draw_thick_line(ImageView& view, const Vec2d& start,
                            const Vec2d& end, double thickness,
                            bool background = false) {
    sf::CircleShape circle;

    sf::Color color = background ? get_bg_color(view) : get_fg_color(view);

    circle.setRadius((float)thickness);
    circle.setFillColor(color);

    circle.setOrigin(sf::Vector2f(circle.getGlobalBounds().width / 2,
                                  circle.getGlobalBounds().height / 2));

    circle.setPosition(to_Vector2f(end));

    view.get_texture().draw(circle);

    circle.setPosition(to_Vector2f(start));

    view.get_texture().draw(circle);

    sf::VertexArray rectangle(sf::PrimitiveType::Quads, 4);

    Vec2d delta = (end - start).normalize();

    rectangle[0].position = to_Vector2f(start + delta.right() * thickness);
    rectangle[1].position = to_Vector2f(start + delta.left() * thickness);
    rectangle[2].position = to_Vector2f(end + delta.left() * thickness);
    rectangle[3].position = to_Vector2f(end + delta.right() * thickness);

    rectangle[0].color = color;
    rectangle[1].color = color;
    rectangle[2].color = color;
    rectangle[3].color = color;

    view.get_texture().draw(rectangle);
}

void LineTool::confirm(ImageView& image_view) {
    if (!active_) return;

    draw_thick_line(image_view, start_pos_, end_pos_, thickness_);

    active_ = false;
    image_view.update_image();
}

static const unsigned PREVIEW_DETAIL = 12;

void CircleTool::render(const MatrixStack<Mat33d>& stack,
                        sf::RenderTarget& target, const ImageView& view) {
    if (!active_) return;

    sf::VertexArray vertices(sf::PrimitiveType::LinesStrip, PREVIEW_DETAIL + 1);

    for (unsigned id = 0; id <= PREVIEW_DETAIL; ++id) {
        Vec2d on_canvas =
            (Vec2d(0.5, 0.5) +
             Vec2d::radial(M_PI * 2.0 * id / PREVIEW_DETAIL, 0.5)) *
                (end_pos_ - start_pos_) +
            start_pos_;
        vertices[id].position =
            to_Vector2f(stack.top() * extrude(view.to_scene_coords(on_canvas)));
        vertices[id].color = PREVIEW_COLOR;
    }

    target.draw(vertices);
}

void CircleTool::on_main(ButtonState state, Vec2d pos, ImageView& view) {
    if (state == BTN_DOWN) {
        start_pos_ = pos;
        end_pos_ = pos;
        active_ = true;
    } else {
        confirm(view);
    }
}

void CircleTool::on_modifier1(ButtonState state, Vec2d pos, ImageView& view) {
    if (state == BTN_DOWN) {
        round_ = true;
    } else {
        round_ = false;
    }

    on_move(pos, view);
}

static double sign(double val) { return val > 0.0 ? 1.0 : -1.0; }

void CircleTool::on_move(Vec2d pos, ImageView& view) {
    end_pos_ = pos;

    Vec2d delta = end_pos_ - start_pos_;

    Vec2d quarter = Vec2d(sign(delta.get_x()), sign(delta.get_y()));

    if (round_) {
        if (abs(delta.get_x()) > abs(delta.get_y())) {
            end_pos_ = start_pos_ +
                       Vec2d(abs(delta.get_x()), abs(delta.get_x())) * quarter;
        } else {
            end_pos_ = start_pos_ +
                       Vec2d(abs(delta.get_y()), abs(delta.get_y())) * quarter;
        }
    }
}

static unsigned DRAW_DETAIL = 128;

void CircleTool::confirm(ImageView& view) {
    Vec2d prev = start_pos_ + (end_pos_ - start_pos_) * Vec2d(1.0, 0.5);

    for (unsigned id = 0; id <= DRAW_DETAIL; ++id) {
        Vec2d on_canvas = (Vec2d(0.5, 0.5) +
                           Vec2d::radial(M_PI * 2.0 * id / DRAW_DETAIL, 0.5)) *
                              (end_pos_ - start_pos_) +
                          start_pos_;
        draw_thick_line(view, prev, on_canvas, thickness_);
        prev = on_canvas;
    }

    active_ = false;

    view.update_image();
}

void PencilTool::render(const MatrixStack<Mat33d>& stack,
                        sf::RenderTarget& target, const ImageView& view) {
    sf::VertexArray vertices(sf::PrimitiveType::LinesStrip, PREVIEW_DETAIL + 1);

    for (unsigned id = 0; id <= PREVIEW_DETAIL; ++id) {
        Vec2d on_canvas = Vec2d::radial(M_PI * 2.0 * id / PREVIEW_DETAIL, 0.5) *
                              thickness_ * 2.0 +
                          end_pos_;
        vertices[id].position =
            to_Vector2f(stack.top() * extrude(view.to_scene_coords(on_canvas)));
        vertices[id].color = PREVIEW_COLOR;
    }

    target.draw(vertices);
}

void PencilTool::on_main(ButtonState state, Vec2d pos, ImageView& view) {
    if (state == BTN_DOWN) {
        active_ = true;
        background_ = false;

        start_pos_ = pos;
    } else {
        cancel();
        view.update_image();
    }
}

void PencilTool::on_sec(ButtonState state, Vec2d pos, ImageView& view) {
    if (state == BTN_DOWN) {
        active_ = true;
        background_ = true;

        start_pos_ = pos;
    } else {
        cancel();
        view.update_image();
    }
}
void PencilTool::on_move(Vec2d pos, ImageView& view) {
    end_pos_ = pos;
    confirm(view);
}

void PencilTool::confirm(ImageView& view) {
    if (!active_) return;

    draw_thick_line(view, start_pos_, end_pos_, thickness_, background_);
    start_pos_ = end_pos_;
}

void StripTool::render(const MatrixStack<Mat33d>& stack,
                       sf::RenderTarget& target, const ImageView& view) {
    if (points_.size() == 0) return;

    sf::VertexArray vertices(sf::PrimitiveType::LinesStrip, 0);

    for (size_t id = 0; id < points_.size(); ++id) {
        sf::Vertex vertex;
        vertex.position = to_Vector2f(
            stack.top() * extrude(view.to_scene_coords(points_[id])));
        vertex.color = PREVIEW_COLOR;
        vertices.append(vertex);
    }

    sf::Vertex last_point;
    last_point.position = to_Vector2f(
        stack.top() *
        extrude(view.to_scene_coords(looped_ ? points_[0] : current_)));
    last_point.color = PREVIEW_COLOR;
    vertices.append(last_point);

    target.draw(vertices);
}

void StripTool::on_main(ButtonState state, Vec2d pos, ImageView& view) {
    current_ = pos;

    if (state == BTN_DOWN) points_.push(pos);
}

void StripTool::on_sec(ButtonState state, Vec2d pos, ImageView& view) {
    if (state == BTN_DOWN && points_.size() > 0) points_.pop();
}

void StripTool::on_modifier1(ButtonState state, Vec2d pos, ImageView& view) {
    looped_ = state == BTN_DOWN;
}

void StripTool::on_move(Vec2d pos, ImageView& view) { current_ = pos; }

void StripTool::cancel() { points_.clear(); }

void StripTool::confirm(ImageView& view) {
    for (size_t id = 0; id + 1 < points_.size(); ++id) {
        draw_thick_line(view, points_[id], points_[id + 1], thickness_);
    }

    if (looped_)
        draw_thick_line(view, points_[0], points_[points_.size() - 1],
                        thickness_);

    points_.clear();
    view.update_image();
}

static const Vec2d PICKER_PREVIEW_SIZE = Vec2d(0.5, 0.5);

void PickerTool::render(const MatrixStack<Mat33d>& stack,
                        sf::RenderTarget& target, const ImageView& view) {
    sf::VertexArray rect(sf::PrimitiveType::Quads, 4);

    Vec2d origin = view.to_scene_coords(pos_) + Vec2d(0.1, 0.1);

    rect[0].position = to_Vector2f(
        stack.top() * extrude(origin + PICKER_PREVIEW_SIZE * Vec2d(0.0, 0.0)));
    rect[1].position = to_Vector2f(
        stack.top() * extrude(origin + PICKER_PREVIEW_SIZE * Vec2d(1.0, 0.0)));
    rect[2].position = to_Vector2f(
        stack.top() * extrude(origin + PICKER_PREVIEW_SIZE * Vec2d(1.0, 1.0)));
    rect[3].position = to_Vector2f(
        stack.top() * extrude(origin + PICKER_PREVIEW_SIZE * Vec2d(0.0, 1.0)));

    sf::Color color =
        sf::Color((unsigned char)color_.get_x(), (unsigned char)color_.get_y(),
                  (unsigned char)color_.get_z());
    for (unsigned id = 0; id < 4; ++id) rect[id].color = color;

    target.draw(rect);
}

void PickerTool::on_main(ButtonState state, Vec2d pos, ImageView& view) {
    pos_ = pos;
    active_ = state == BTN_DOWN;

    if (state == BTN_DOWN) {
        density_++;
        background_ = false;
    }
}

void PickerTool::on_sec(ButtonState state, Vec2d pos, ImageView& view) {
    pos_ = pos;
    active_ = state == BTN_DOWN;

    if (state == BTN_DOWN) {
        density_++;
        background_ = true;
    }
}

void PickerTool::on_move(Vec2d pos, ImageView& view) {
    pos_ = pos;

    const sf::Image& image = view.get_image();
    sf::Color color_raw = sf::Color::Black;
    if (pos_.get_x() > 0.0 && pos_.get_x() < image.getSize().x &&
        pos_.get_y() > 0.0 && pos_.get_y() < image.getSize().y) {
        color_raw =
            image.getPixel((unsigned)pos_.get_x(), (unsigned)pos_.get_y());
    }

    Vec3d current = Vec3d(color_raw.r, color_raw.g, color_raw.b);

    if (density_ == 0) {
        color_ = current;
    }

    if (active_) {
        color_ = current * (1.0 / (density_ + 1)) +
                 color_ * ((double)density_ / (density_ + 1));

        density_++;
    }
}

void PickerTool::cancel() {
    active_ = false;
    density_ = 0;
}

void PickerTool::confirm(ImageView& view) {
    sf::Color color =
        sf::Color((unsigned char)color_.get_x(), (unsigned char)color_.get_y(),
                  (unsigned char)color_.get_z());

    if (background_)
        view.get_pallet()->set_bg_color(color);
    else
        view.get_pallet()->set_fg_color(color);
}
