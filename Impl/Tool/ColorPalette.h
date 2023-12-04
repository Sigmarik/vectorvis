/**
 * @file ColorPalette.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Color palette implementation
 * @version 0.1
 * @date 2023-12-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __IMPL_TOOL_COLOR_PALETTE_H
#define __IMPL_TOOL_COLOR_PALETTE_H

#include "Plug/Tool/Tool.h"

class ColorPalette : public plug::ColorPalette {
   public:
    plug::Color getFGColor(void) const override { return foreground_; }

    plug::Color getBGColor(void) const override { return background_; }

    void setFGColor(plug::Color new_fg_color) override {
        foreground_ = new_fg_color;
    }

    void setBGColor(plug::Color new_bg_color) override {
        background_ = new_bg_color;
    }

   private:
    plug::Color foreground_ = plug::Color(0, 0, 0, 255),
                background_ = plug::Color(255, 255, 255, 255);
};

#endif