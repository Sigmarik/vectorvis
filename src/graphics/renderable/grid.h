/**
 * @file grid.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Grid renderable object
 * @version 0.1
 * @date 2023-09-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GRID_H
#define GRID_H

#include "renderable.h"

struct Grid : public Renderable {
    Grid(const Vec2d& x_unit, const Vec2d& y_unit);

    void render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target);

    void set_x_unit(const Vec2d& vector);
    void set_y_unit(const Vec2d& vector);

    Vec2d get_x_unit() const;
    Vec2d get_y_unit() const;

   private:
    Mat33d matrix();

    Vec2d x_unit_, y_unit_;
};

#endif
