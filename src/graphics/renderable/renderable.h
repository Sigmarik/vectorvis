/**
 * @file renderable.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Abstract renderable object
 * @version 0.1
 * @date 2023-09-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "geometry/matrix_stack.h"

struct AssetShelf final {
    sf::Font font;
};

struct Renderable {
    virtual ~Renderable() = default;
    virtual void render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                        const AssetShelf& assets) = 0;
};

#endif
