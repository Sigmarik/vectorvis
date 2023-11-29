/**
 * @file sf_cheatsheet.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Cheatsheet of useful SFML2Plug functions
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __GRAPHICS_SF_CHEATSHEET_H
#define __GRAPHICS_SF_CHEATSHEET_H

#include <SFML/Graphics/Image.hpp>

#include "Plug/Graphics/Texture.h"

static void to_plug_texture(sf::Image& sf_image, plug::Texture& plug_texture) {
    //! TODO: Replace with casual setter call {

    delete plug_texture.data;

    new (&plug_texture)
        plug::Texture(sf_image.getSize().x, sf_image.getSize().y);

    //! }

    const sf::Uint8* data = sf_image.getPixelsPtr();

    for (unsigned id_x = 0; id_x < plug_texture.width; ++id_x) {
        for (unsigned id_y = 0; id_y < plug_texture.height; ++id_y) {
            sf::Color pixel = sf_image.getPixel(id_x, id_y);
            plug_texture.setPixel(
                id_x, id_y, plug::Color(pixel.r, pixel.g, pixel.b, pixel.a));
        }
    }
}

static void to_sf_image(const plug::Texture& data, sf::Image& image) {
    image.create((unsigned)data.width, (unsigned)data.height);

    for (unsigned id_x = 0; id_x < data.width; ++id_x) {
        for (unsigned id_y = 0; id_y < data.height; ++id_y) {
            plug::Color pixel = data.getPixel(id_x, id_y);

            image.setPixel(id_x, id_y,
                           sf::Color(pixel.r, pixel.g, pixel.b, pixel.a));
        }
    }
}

#endif
