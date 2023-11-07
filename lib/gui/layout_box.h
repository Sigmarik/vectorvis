/**
 * @file layout_box.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Widget behavior descriptor
 * @version 0.1
 * @date 2023-11-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef LAYOUT_BOX_H
#define LAYOUT_BOX_H

#include "geometry/vector.hpp"

enum Corner {
    TOP_LEFT = 0,
    TOP_RIGHT = 1,
    BOTTOM_LEFT = 2,
    BOTTOM_RIGHT = 3,
};

struct LayoutBox {
    virtual ~LayoutBox() = default;
    virtual Vec2d get_position() const = 0;
    virtual bool set_position(Vec2d position) = 0;
    virtual Vec2d get_size() const = 0;
    virtual bool set_size(const Vec2d&) = 0;
    virtual void on_parent_update(const LayoutBox& box) = 0;
    virtual LayoutBox* clone() const = 0;

    Vec2d get_corner(Corner corner) const;
};

#endif
