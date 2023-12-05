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
#include "Impl/Tool/ColorPalette.h"
#include "Impl/Widget.h"

struct FilterWindow;

struct CanvasView : public Widget {
    CanvasView(const plug::LayoutBox& box, Canvas& canvas);
    CanvasView(const CanvasView& view) = default;
    ~CanvasView() = default;

    CanvasView& operator=(const CanvasView& view) = default;

    Canvas& getCanvas() const { return canvas_; }

    void draw(plug::TransformStack& stack, plug::RenderTarget& target) override;

    void onEvent(const plug::Event& event, plug::EHC& context) override;

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

    void setFilterView(FilterWindow& view) { filter_view_ = &view; }
    void hideFilterView() { filter_view_ = nullptr; }

   private:
    plug::Transform getCanvasCoords() const;

    Vec2d getPixelPos(const Vec2d& screen_pos,
                      const plug::TransformStack& stack);

    Canvas& canvas_;
    ColorPalette palette_ = ColorPalette();

    FilterWindow* filter_view_ = nullptr;
};

#endif
