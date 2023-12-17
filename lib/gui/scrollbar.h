/**
 * @file scrollbar.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Scrollbar widget
 * @version 0.1
 * @date 2023-12-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include "gui.h"

enum class ScrollbarType {
    Vertical,
    Horizontal,
};

struct Scrollbar;

struct ScrollbarBtn : public Button {
    ScrollbarBtn(bool additive, Scrollbar& owner, const plug::LayoutBox& box);

    void onPush() override;

    void onTick(const plug::TickEvent& event, plug::EHC& context) override;

   private:
    Scrollbar& owner_;
    bool additive_ = false;

    long long unsigned push_time_ = 0;
};

struct Scrollbit : public Button {
    using Button::Button;

    void onMouseMove(const plug::MouseMoveEvent& event,
                     plug::EHC& context) override;

   private:
    Vec2d old_mouse_pos_ = Vec2d(0.0, 0.0);
};

struct Scrollbar : public Panel {
    Scrollbar(ScrollbarType type, const Vec2d& center, double length,
              const Vec2d& bind = Vec2d(0.0, 0.0));

    void onEvent(const plug::Event& event, plug::EHC& context) override;

    virtual void onUpdate(double value){};

    void setValue(double value);
    double getValue() const { return value_; }

    ScrollbarType getType() const { return type_; }

    void setText(const char* text) { scrollbit_.setText(text); }
    const char* getText() const { return scrollbit_.getText(); }

   private:
    const ScrollbarType type_;

    double value_ = 0.5;

    ScrollbarBtn up_btn_, down_btn_;
    Scrollbit scrollbit_;
};

#endif
