#include "interactive.h"

#include "sf_cheatsheet.hpp"

Vec2d Interactive::get_center() const { return center_; }
Vec2d Interactive::get_size() const { return size_; }
void Interactive::move(const Vec2d& new_center) { center_ = new_center; }
void Interactive::resize(const Vec2d& new_size) { size_ = new_size; }

bool Interactive::is_under(const MatrixStack<Mat33d>& stack, const Vec2d& pos) {
    Vec3d center = extrude(center_);
    Vec3d size = extrude(size_);

    // clang-format off
    Vec3d top_left      = stack.top() * (center + size * Vec3d(-0.5, 0.5, 0.0));
    Vec3d top_right     = stack.top() * (center + size * Vec3d(0.5, 0.5, 0.0));
    Vec3d bottom_right  = stack.top() * (center + size * Vec3d(0.5, -0.5, 0.0));
    Vec3d bottom_left   = stack.top() * (center + size * Vec3d(-0.5, -0.5, 0.0));

    Vec2d tl(top_left.get_x(),      top_left.get_y());
    Vec2d tr(top_right.get_x(),     top_right.get_y());
    Vec2d br(bottom_right.get_x(),  bottom_right.get_y());
    Vec2d bl(bottom_left.get_x(),   bottom_left.get_y());

    bool top_check      = cross(tl - tr, pos - tr) <= 0.0;
    bool right_check    = cross(tr - br, pos - br) <= 0.0;
    bool bottom_check   = cross(br - bl, pos - bl) <= 0.0;
    bool left_check     = cross(bl - tl, pos - tl) <= 0.0;
    // clang-format on

    return top_check && right_check && bottom_check && left_check;
}
