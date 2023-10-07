/**
 * @file tools.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Image editing tools
 * @version 0.1
 * @date 2023-10-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef IMG_EDIT_TOOLS_H
#define IMG_EDIT_TOOLS_H

#include <SFML/Graphics/Image.hpp>

#include "geometry/vector.hpp"

struct ImgTool {
    ImgTool(unsigned radius, double strength)
        : radius_(radius), strength_(strength) {}
    virtual ~ImgTool() = default;

    virtual void use(sf::Image& image, const Vec2d& pos) = 0;

    unsigned get_radius() const { return radius_; }
    void set_radius(unsigned radius) { radius_ = radius; }

    double get_strength() const { return strength_; }
    void set_strength(double strength) { strength_ = strength; }

   protected:
    unsigned radius_;
    double strength_;
};

struct EraserTool : public ImgTool {
    using ImgTool::ImgTool;

    void use(sf::Image& image, const Vec2d& pos) override;
};

#endif
