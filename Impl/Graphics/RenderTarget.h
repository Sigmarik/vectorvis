/**
 * @file RenderTarget.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief
 * @version 0.1
 * @date 2023-11-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __IMPL_RENDER_TARGET_H
#define __IMPL_RENDER_TARGET_H

#include <SFML/Graphics/RenderTarget.hpp>

#include "Plug/Graphics.h"
#include "inttypes.h"

static const unsigned INVALID_NATIVE_HANDLE = UINT32_MAX;

struct RenderTarget : plug::RenderTarget {
    RenderTarget(sf::RenderTarget &target) : base_(target) {}
    ~RenderTarget(void) = default;

    void draw(const plug::VertexArray &array) override;

    void draw(const plug::VertexArray &array,
              const plug::Texture &texture) override;

    void clear(plug::Color color) override;

    void setActive(bool active) override;

   private:
    sf::RenderTarget &base_;
};

#endif
