/**
 * @file Canvas.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Canvas implementation
 * @version 0.1
 * @date 2023-11-28
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __IMPL_CANVAS_CANVAS_H
#define __IMPL_CANVAS_CANVAS_H

#include <SFML/Graphics/RenderTexture.hpp>

#include "Plug/Canvas/Canvas.h"
#include "Plug/Graphics/Texture.h"
#include "SelectionMask.h"

class Canvas : public plug::Canvas {
   public:
    Canvas(size_t width, size_t height);
    Canvas(const plug::Texture &texture);
    Canvas(const char *path);

    void draw(const plug::VertexArray &vertex_array) override;

    void draw(const plug::VertexArray &vertex_array,
              const plug::Texture &texture) override;

    plug::Vec2d getSize(void) const override {
        return Vec2d(cpu_content_.getSize().x, cpu_content_.getSize().y);
    }

    void setSize(const plug::Vec2d &size) override;

    plug::SelectionMask &getSelectionMask(void) override { return mask_; }

    plug::Color getPixel(size_t x, size_t y) const override;

    void setPixel(size_t x, size_t y, const plug::Color &color) override;

    const plug::Texture &getTexture(void) const override;

   private:
    void synchGPU() const;
    void synchCPU() const;

    mutable bool cpu_sided_ = true;

    mutable sf::RenderTexture gpu_content_ = sf::RenderTexture();
    mutable sf::Image cpu_content_ = sf::Image();

    SelectionMask mask_;
};

#endif
