/**
 * @file tool_selector_btn.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Tool selection button
 * @version 0.1
 * @date 2023-12-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __GUI_BUTTONS_TOOL_SELECTOR_BTN_H
#define __GUI_BUTTONS_TOOL_SELECTOR_BTN_H

#include "gui/gui.h"

struct ToolSelectionButton : public Button {
    ToolSelectionButton(size_t id, const plug::LayoutBox& box)
        : Button(box), id_(id) {}

    void onPush() override;

    void draw(plug::TransformStack& stack, plug::RenderTarget& target) override;

   private:
    size_t id_;
};

#endif
