/**
 * @file arrow.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Arrow element
 * @version 0.1
 * @date 2023-09-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ARROW_H
#define ARROW_H

#include "renderable.h"

struct Arrow : public Renderable {
    explicit Arrow();
    explicit Arrow(const Vec2d& vector);
    ~Arrow();

    void render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target) override;

    Vec2d get_vector();
    void set_vector(const Vec2d& vector);

   private:
    Vec2d vector_;
};

#endif
