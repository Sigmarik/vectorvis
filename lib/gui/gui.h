/**
 * @file gui.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief GUI elements
 * @version 0.1
 * @date 2023-09-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GUI_H
#define GUI_H

#include "Impl/Widget.h"
#include "Plug/Math.h"
#include "data_structures/stack.h"
#include "graphics/AssetShelf.h"

struct Panel : public Widget {
    using Widget::Widget;

    ~Panel() = default;

    void draw(plug::TransformStack& stack, plug::RenderTarget& target) override;

    void onEvent(const plug::Event& event, plug::EHC& context) override;

    void onMouseMove(const plug::MouseMoveEvent& event,
                     plug::EHC& context) override;

    void addChild(plug::Widget& widget);

    void onParentUpdate(const plug::LayoutBox& parent_box) override;

    bool doesFollowMouse() const { return follows_mouse_; }
    void setFollowsMouse(bool follows) {
        follows_mouse_ = follows;
        is_mp_valid_ = false;
    }

    DesignId getDesign() const { return design_; }
    void setDesign(DesignId design) { design_ = design; }

   private:
    plug::Transform getLocalCoords() const;

    void constructMesh(plug::VertexArray& array,
                       const plug::TransformStack& stack);

    Stack<plug::Widget*> children_ = Stack<plug::Widget*>();

    bool follows_mouse_ = false;

    Vec2d known_mouse_pos_ = Vec2d(0.0, 0.0);
    DesignId design_ = DSGN_PANEL_DEFAULT;

    Vec2d old_mouse_pos_ = Vec2d(0.0, 0.0);
    bool is_mp_valid_ = false;
};

struct Button : public Widget {
    using Widget::Widget;
    Button(const Button& button) = default;
    ~Button() = default;

    Button& operator=(const Button& button) = default;

    void draw(plug::TransformStack& stack, plug::RenderTarget& target) override;

    virtual void onPush(){};
    virtual void onRelease(){};
    virtual void onHover(){};
    virtual void onUnhover(){};

    void onTick(const plug::TickEvent& event, plug::EHC& context) override;

    void onMouseMove(const plug::MouseMoveEvent& event,
                     plug::EHC& context) override;

    void onMousePressed(const plug::MousePressedEvent& event,
                        plug::EHC& context) override;

    void onMouseReleased(const plug::MouseReleasedEvent& event,
                         plug::EHC& context) override;

    DesignId getDesign() const { return design_; }
    void setDesign(DesignId id) { design_ = id; }

   private:
    const char* text_ = "DEFAULT TEXT";

    bool is_pushed_ = false;
    bool is_hovered_ = false;
    unsigned long long hover_timer_ = 0;
    unsigned long long push_timer_ = 0;
    unsigned long long last_upd_time_ = 0;
    Vec2d known_mouse_pos_ = Vec2d(0.0, 0.0);
    DesignId design_ = DSGN_PANEL_DEFAULT;
};

struct DragButton : public Button {
    DragButton(Panel& panel);

    void onPush() override;
    void onRelease() override;

   private:
    Panel& panel_;
};

#endif
