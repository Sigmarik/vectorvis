/**
 * @file window.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Panel-related classes
 * @version 0.1
 * @date 2023-09-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef WINDOW_H
#define WINDOW_H

#include "geometry/vector.h"

struct Window {
    Window();
    Window(const Vec2d& top_left, const Vec2d& bottom_right);
    ~Window();
};

#endif
