/**
 * @file Texture.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief Texture data structure
 *
 * @version 0.1
 * @date 2023-11-16
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __PLUG_GRAPHICS_TEXTURE_H
#define __PLUG_GRAPHICS_TEXTURE_H

#include <cstddef>

#include "Plug/Color.h"

namespace plug {

/**
 * @brief Array of colors (stored on the CPU)
 *
 */
struct Texture {
    Texture(size_t width_, size_t height_, Color *data_)
        : data(data_), width(width_), height(height_) {}

    Texture(size_t width_, size_t height_)
        : data(new Color[width_ * height_]), width(width_), height(height_) {}

    /**
     * @brief Image data
     *
     */
    Color *data;

    /**
     * @brief Image width (in pixels)
     *
     */
    size_t width;

    /**
     * @brief Image height (in pixels)
     *
     */
    size_t height;
};

}  // namespace plug

#endif /* Texture.h */
