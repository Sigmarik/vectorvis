/**
 * @file color_picker.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Color picker gui part
 * @version 0.1
 * @date 2023-12-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef COLOR_PICKER_H
#define COLOR_PICKER_H

#include "Impl/Tool/ColorPalette.h"
#include "gui/dropdown_list.h"
#include "gui/scrollbar.h"

enum class PickerType { Foreground, Background };

struct ColorPicker : public DropdownList {
    ColorPicker(ColorPalette& palette, PickerType type,
                const plug::LayoutBox& box);

    void draw(plug::TransformStack& stack, plug::RenderTarget& target) override;

    plug::Color getColor() const;

    void onClose() override;

   private:
    ColorPalette& subject_;
    PickerType type_;

    Panel editor_;
    Scrollbar red_bar_;
    Scrollbar green_bar_;
    Scrollbar blue_bar_;
};

#endif
