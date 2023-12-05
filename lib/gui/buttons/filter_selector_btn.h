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

#include "filter_window.h"
#include "gui/canvas_view.h"
#include "gui/gui.h"

struct FilterSelectionButton : public Button {
    FilterSelectionButton(size_t id, CanvasView& view,
                          const plug::LayoutBox& box);

    void onPush() override;

    void draw(plug::TransformStack& stack, plug::RenderTarget& target) override;

   private:
    size_t id_;
    CanvasView& canvas_;
    FilterWindow filter_view_;
};

#endif
