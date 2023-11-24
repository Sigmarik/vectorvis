/**
 * @file environment.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Execution environment structure
 * @version 0.1
 * @date 2023-11-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __GRAPHICS_ENVIRONMENT_H
#define __GRAPHICS_ENVIRONMENT_H

#include "Plug/Math.h"

struct Environment {
    static void setScreenSize(const Vec2d& size) { screen_size_ = size; }
    static Vec2d getScreenSize() { return screen_size_; }

   private:
    Environment() { screen_size_ = Vec2d(1.0, 1.0); }

    static Environment& getInstance();

    static Vec2d screen_size_;
};

#endif
