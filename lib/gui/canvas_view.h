/**
 * @file canvas_view.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Canvas view widget
 * @version 0.1
 * @date 2023-11-28
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __GUI_CANVAS_VIEW_H
#define __GUI_CANVAS_VIEW_H

#include "Impl/Widget.h"
#include "Plug/Canvas.h"

struct CanvasView : public Widget {
    CanvasView(const LayoutBox& box, Canvas& canvas);

    void draw(plug::TransformStack& stack, plug::RenderTarget& target) override;

    void onMouseMove(const plug::MouseMoveEvent& event,
                     plug::EHC& context) override;

    void onMousePressed(const plug::MousePressedEvent& event,
                        plug::EHC& context) override;

    void onMouseReleased(const plug::MouseReleasedEvent& event,
                         plug::EHC& context) override;

    void onKeyboardPressed(const KeyboardPressedEvent& event,
                           EHC& context) override;

   private:
    Canvas& canvas_;
};

#endif
