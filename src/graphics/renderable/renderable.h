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
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "geometry/matrix_stack.h"

struct DesignDescriptor {
    void load(const char* path);

    sf::Texture atlas = sf::Texture();
    sf::Shader shader = sf::Shader();
};

struct AssetShelf final {
    AssetShelf();

    bool shaders_available = false;

    sf::Font font;
    DesignDescriptor button_design;
    DesignDescriptor panel_design;
};

struct Renderable {
    virtual ~Renderable() = default;
    virtual void render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                        const AssetShelf& assets) = 0;
};

#endif
