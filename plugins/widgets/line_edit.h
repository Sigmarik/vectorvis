/**
 * @file line_edit.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Line editor for curves filter
 * @version 0.1
 * @date 2023-12-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef LINE_EDIT_H
#define LINE_EDIT_H

#include "Impl/Widget.h"
#include "data_structures/stack.h"

struct SplinePoint : public Widget {
    SplinePoint(Vec2d position, Vec2d frame_size);

    using Widget::draw;

    void onMouseMove(const plug::MouseMoveEvent& event,
                     plug::EHC& context) override;

    void onMousePressed(const plug::MousePressedEvent& event,
                        plug::EHC& context) override;

    void onMouseReleased(const plug::MouseReleasedEvent& event,
                         plug::EHC& context) override;

    Vec2d getPos() const { return getLayoutBox().getPosition(); }

    bool getAlive() const { return alive_; }

    void setFixed(bool fixed) { fixed_ = fixed; }
    bool getFixed() const { return fixed_; }

   private:
    bool fixed_ = false;
    bool follows_mouse_ = false;
    bool alive_ = true;

    Vec2d frame_size_;

    Vec2d mouse_pos_ = Vec2d(0.0, 0.0);
};

struct LineEdit : public Widget {
    LineEdit(const plug::LayoutBox& box);

    void draw(plug::TransformStack& stack, plug::RenderTarget& target) override;
    void onEvent(const plug::Event& event, plug::EHC& context) override;
    void onMousePressed(const plug::MousePressedEvent& event,
                        plug::EHC& context) override;

    double getAt(double input) const;
    unsigned char getValue(unsigned char value) const;

   private:
    void sortPoints();

    Stack<SplinePoint> points_ = Stack<SplinePoint>();
};

#endif
