#include "interactive.h"

#include "sf_cheatsheet.hpp"

Interactive::Interactive(const Vec2d& center, const Vec2d& size) {
    center_ = center;
    size_ = size;
    vis_center_ = center;
    vis_size_ = size;
}

void Interactive::on_event(MatrixStack<Mat33d>& stack,
                           Interaction interaction) {
    if (interaction.event->type != sf::Event::MouseMoved) return;
    sf::Vector2i pos = sf::Mouse::getPosition(*interaction.window);
    mouse_position_ = Vec2d((double)pos.x, (double)pos.y);
}

void Interactive::apply_anchor(const Vec2d& parent_size) {
    bool split_x = abs(anchor_.get_size().get_x()) > 1e-5;
    bool split_y = abs(anchor_.get_size().get_y()) > 1e-5;

    Vec2d new_bl = -parent_size / 2.0 + Vec2d(0.5, 0.5) + anchor_.get_center() -
                   anchor_.get_size() / 2.0;
    Vec2d new_tr = parent_size / 2.0 - (Vec2d(0.5, 0.5) - anchor_.get_center() -
                                        anchor_.get_size() / 2.0);

    if (!split_x) {
        double new_x = anchor_.get_center().get_x() * parent_size.get_x();
        new_bl.set_x(new_x);
        new_tr.set_x(new_x);
    }
    if (!split_y) {
        double new_y = anchor_.get_center().get_y() * parent_size.get_y();
        new_bl.set_y(new_y);
        new_tr.set_y(new_y);
    }

    Vec2d new_center = (new_bl + new_tr) / 2.0;
    Vec2d new_size = (new_tr - new_bl);

    vis_size_.set_x(split_x ? new_size.get_x() / anchor_.get_size().get_x()
                            : 1.0);
    vis_size_.set_y(split_y ? new_size.get_y() / anchor_.get_size().get_y()
                            : 1.0);
    vis_center_ = new_center + center_ * vis_size_;
    vis_size_ = vis_size_ * size_;
}

// bool Interactive::is_under(const MatrixStack<Mat33d>& stack, const Vec2d&
// pos) {
//     // clang-format off
//     Vec2d tl = get_corner(TOP_LEFT,     stack);
//     Vec2d tr = get_corner(TOP_RIGHT,    stack);
//     Vec2d br = get_corner(BOTTOM_RIGHT, stack);
//     Vec2d bl = get_corner(BOTTOM_LEFT,  stack);

//     bool top_check      = cross(tl - tr, pos - tr) <= 0.0;
//     bool right_check    = cross(tr - br, pos - br) <= 0.0;
//     bool bottom_check   = cross(br - bl, pos - bl) <= 0.0;
//     bool left_check     = cross(bl - tl, pos - tl) <= 0.0;
//     // clang-format on

//     return top_check && right_check && bottom_check && left_check;
// }

// Vec2d Interactive::get_corner(Corner corner, const MatrixStack<Mat33d>&
// stack) {
//     Vec2d direction =
//         Vec2d((corner & 1) ? 0.5 : -0.5, (corner & 2) ? -0.5 : 0.5);
//     Vec3d transformed =
//         stack.top() * extrude(vis_center_ + vis_size_ * direction);
//     return Vec2d(transformed.get_x(), transformed.get_y()) /
//            transformed.get_z();
// }

unsigned long long WorldTimer::get() {
    static WorldTimer* instance = NULL;

    if (instance == NULL) {
        instance = (WorldTimer*)calloc(1, sizeof(*instance));
        instance->clock_ = sf::Clock();
    }

    return (unsigned long long)instance->clock_.getElapsedTime()
        .asMicroseconds();
}
