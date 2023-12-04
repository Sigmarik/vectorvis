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

#include "Impl/Canvas/Canvas.h"
#include "Impl/Widget.h"

struct CanvasView : public Widget {
    CanvasView(const plug::LayoutBox& box, Canvas& canvas);

    void draw(plug::TransformStack& stack, plug::RenderTarget& target) override;

    void onMouseMove(const plug::MouseMoveEvent& event,
                     plug::EHC& context) override;

    void onMousePressed(const plug::MousePressedEvent& event,
                        plug::EHC& context) override;

    void onMouseReleased(const plug::MouseReleasedEvent& event,
                         plug::EHC& context) override;

    void onKeyboardPressed(const plug::KeyboardPressedEvent& event,
                           plug::EHC& context) override;

    void onKeyboardReleased(const plug::KeyboardReleasedEvent& event,
                            plug::EHC& context) override;

   private:
    Vec2d getPixelPos(const Vec2d& screen_pos,
                      const plug::TransformStack& stack);

    Canvas& canvas_;
};

#endif
