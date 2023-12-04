/**
 * @file filter_selector_btn.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Filter selection button
 * @version 0.1
 * @date 2023-12-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __GUI_BUTTONS_FILTER_SELECT_BTN_H
#define __GUI_BUTTONS_FILTER_SELECT_BTN_H

#include "gui/gui.h"

struct FilterSelectionButton : public Button {
    FilterSelectionButton(size_t id, const plug::LayoutBox& box)
        : Button(box), id_(id) {}

    void onPush() override;

    void draw(plug::TransformStack& stack, plug::RenderTarget& target) override;

   private:
    size_t id_;
};

#endif
