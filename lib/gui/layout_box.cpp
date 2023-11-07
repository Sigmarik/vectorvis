#include "layout_box.h"

Vec2d LayoutBox::get_corner(Corner corner) const {
    Vec2d direction =
        Vec2d((corner & 1) ? 0.5 : -0.5, (corner & 2) ? -0.5 : 0.5);
    return get_position() + get_size() * direction;
}
