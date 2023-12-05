/**
 * @file filter_window.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Filter wrapper window
 * @version 0.1
 * @date 2023-12-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __GUI_BUTTONS_FILTER_WINDOW_H
#define __GUI_BUTTONS_FILTER_WINDOW_H

#include "Plug/Filter.h"
#include "gui/canvas_view.h"
#include "gui/gui.h"

struct FilterWindow;

struct FilterApplyButton : Button {
    FilterApplyButton(FilterWindow& parent, const plug::LayoutBox& box)
        : Button(box), parent_(parent) {}

    void onPush() override;

   private:
    FilterWindow& parent_;
};

struct FilterCancelButton : Button {
    FilterCancelButton(FilterWindow& parent, const plug::LayoutBox& box)
        : Button(box), parent_(parent) {}

    void onPush() override;

   private:
    FilterWindow& parent_;
};

struct FilterSettingsHolder : public Panel {
    FilterSettingsHolder(plug::Filter& filter);

    void draw(plug::TransformStack& stack, plug::RenderTarget& target) override;

   private:
    plug::Filter& filter_;
};

struct FilterWindow : public Panel {
    FilterWindow(CanvasView& canvas_view, plug::Filter& filter);

    void apply();
    void cancel();

    bool getActive() const { return active_; }
    void setActive(bool active) { active_ = active; }

    CanvasView& getCanvasView() const { return canvas_; }
    plug::Filter& getFilter() const { return filter_; }

   private:
    CanvasView& canvas_;
    plug::Filter& filter_;

    FilterApplyButton apply_btn_;
    FilterCancelButton cancel_btn_;

    FilterSettingsHolder filter_view_;

    bool active_ = false;
};

#endif
