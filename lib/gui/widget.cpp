#include "widget.h"

#include "sf_cheatsheet.hpp"

#define PROPAGATE_EVENT(event, ...)                              \
    do {                                                         \
        for (size_t id = children_.size(); id > 0; --id) {       \
            bool status = children_[id - 1]->event(__VA_ARGS__); \
            if (status) {                                        \
                focus(*children_[id - 1]);                       \
                return true;                                     \
            }                                                    \
        }                                                        \
        return false;                                            \
    } while (0)

#define PROPAGATE_POSITIONAL_EVENT(event, ...)                   \
    do {                                                         \
        stack.push(get_local_space());                           \
        for (size_t id = children_.size(); id > 0; --id) {       \
            bool status = children_[id - 1]->event(__VA_ARGS__); \
            if (status) {                                        \
                focus(*children_[id - 1]);                       \
                stack.pop();                                     \
                return true;                                     \
            }                                                    \
        }                                                        \
        stack.pop();                                             \
        return false;                                            \
    } while (0)

Widget::Widget(const LayoutBox& box) : Renderable(), box_(box.clone()) {}

Widget::Widget(const Widget& widget) : Renderable(), box_(nullptr) {
    operator=(widget);
}

Widget& Widget::operator=(const Widget& widget) {
    if (box_ != nullptr) delete box_;
    box_ = widget.box_->clone();
    return *this;
}

Widget::~Widget() {
    if (box_) delete box_;
}

void Widget::render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                    const AssetShelf& assets) {
    stack.push(get_local_space());
    for (size_t id = 0; id < children_.size(); ++id) {
        children_[id]->render(stack, target, assets);
    }
    stack.pop();
}

bool Widget::on_mouse_move(const Vec2d& mouse_pos, TransformStack& stack) {
    PROPAGATE_POSITIONAL_EVENT(on_mouse_move, mouse_pos, stack);
}

bool Widget::on_mouse_button_pressed(const Vec2d& mouse_pos, MouseButton button,
                                     TransformStack& stack) {
    PROPAGATE_POSITIONAL_EVENT(on_mouse_button_pressed, mouse_pos, button,
                               stack);
}

bool Widget::on_mouse_button_released(const Vec2d& mouse_pos,
                                      MouseButton button,
                                      TransformStack& stack) {
    PROPAGATE_POSITIONAL_EVENT(on_mouse_button_released, mouse_pos, button,
                               stack);
}

bool Widget::on_key_pressed(KeyboardKey key) {
    PROPAGATE_EVENT(on_key_pressed, key);
}

bool Widget::on_key_released(KeyboardKey key) {
    PROPAGATE_EVENT(on_key_released, key);
}

bool Widget::on_tick(double delta_time) {
    PROPAGATE_EVENT(on_tick, delta_time);
}

bool Widget::on_any_event(const sf::Event& event, const sf::Window& window,
                          TransformStack& stack) {
    PROPAGATE_POSITIONAL_EVENT(on_any_event, event, window, stack);
}

void Widget::on_parent_update(const LayoutBox& parent_box) {
    box_->on_parent_update(parent_box);
    for (size_t id = 0; id < children_.size(); ++id) {
        children_[id]->on_parent_update(*box_);
    }
}

Mat33d Widget::get_local_space() const {
    Vec2d center = box_->get_position();
    // clang-format off
    return Mat33d(
        1.0, 0.0, center.get_x(),
        0.0, 1.0, center.get_y(),
        0.0, 0.0, 1.0
    );
    // clang-format on
}

void Widget::add_child(Widget& widget) {
    if (children_.size() > 0) children_.top()->focused_ = false;
    children_.push(&widget);
    widget.focused_ = true;
}

void Widget::focus(Widget& widget) {
    if (children_.size() == 0) return;

    children_.top()->focused_ = false;
    for (unsigned id = 0; id + 1 < children_.size(); ++id) {
        if (children_[id] != &widget) continue;

        children_[id] = children_[id + 1];
        children_[id] = &widget;
    }
    children_.top()->focused_ = true;
}

void Widget::dispatch_input(TransformStack& stack, const sf::Event& event,
                            const sf::Window& window) {
    sf::Vector2i mouse_raw_pos = sf::Mouse::getPosition(window);
    Vec2d mouse_pos((double)mouse_raw_pos.x / window.getSize().x,
                    (double)mouse_raw_pos.y / window.getSize().y);
    switch (event.type) {
        case sf::Event::MouseMoved: {
            on_mouse_move(mouse_pos, stack);
        } break;
        case sf::Event::MouseButtonPressed: {
            on_mouse_button_pressed(mouse_pos, event.mouseButton.button, stack);
        } break;
        case sf::Event::MouseButtonReleased: {
            on_mouse_button_released(mouse_pos, event.mouseButton.button,
                                     stack);
        } break;
        case sf::Event::KeyPressed: {
            on_key_pressed(event.key.code);
        } break;
        case sf::Event::KeyReleased: {
            on_key_released(event.key.code);
        } break;
        default:
            break;
    }

    on_any_event(event, window, stack);
}

Vec2d Widget::get_corner(Corner corner, TransformStack stack) const {
    Vec2d box_corner = box_->get_corner(corner);
    Vec3d projected = stack.top() * extrude(box_corner);
    return Vec2d(projected.get_x(), projected.get_y());
}

bool Widget::is_under(TransformStack& stack, const Vec2d& screen_pos) const {
    if (box_->get_size().get_x() == 0.0 || box_->get_size().get_y() == 0.0)
        return false;

    // clang-format off
    Vec2d tl = get_corner(TOP_LEFT,     stack);
    Vec2d tr = get_corner(TOP_RIGHT,    stack);
    Vec2d br = get_corner(BOTTOM_RIGHT, stack);
    Vec2d bl = get_corner(BOTTOM_LEFT,  stack);

    bool top_check      = cross(tl - tr, screen_pos - tr) <= 0.0;
    bool right_check    = cross(tr - br, screen_pos - br) <= 0.0;
    bool bottom_check   = cross(br - bl, screen_pos - bl) <= 0.0;
    bool left_check     = cross(bl - tl, screen_pos - tl) <= 0.0;
    // clang-format on

    return top_check && right_check && bottom_check && left_check;
}
