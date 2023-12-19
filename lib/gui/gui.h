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

struct Designable : public Widget {
    using Widget::Widget;

    DesignId getDesign() const { return design_; }
    void setDesign(DesignId design) { design_ = design; }

    void constructMesh(plug::VertexArray& array,
                       const plug::TransformStack& stack);

   private:
    DesignId design_ = DSGN_PANEL_DEFAULT;
};

struct Panel : public Designable {
    using Designable::Designable;

    ~Panel() = default;

    void draw(plug::TransformStack& stack, plug::RenderTarget& target) override;

    void onEvent(const plug::Event& event, plug::EHC& context) override;

    void onMouseMove(const plug::MouseMoveEvent& event,
                     plug::EHC& context) override;

    void onMousePressed(const plug::MousePressedEvent& event,
                        plug::EHC& context) override;

    void addChild(plug::Widget& widget);

    void onParentUpdate(const plug::LayoutBox& parent_box) override;

    bool doesFollowMouse() const { return follows_mouse_; }
    void setFollowsMouse(bool follows) {
        follows_mouse_ = follows;
        is_mp_valid_ = false;
    }

   protected:
    Stack<plug::Widget*> children_ = Stack<plug::Widget*>();

   private:
    bool follows_mouse_ = false;

    Vec2d known_mouse_pos_ = Vec2d(0.0, 0.0);

    Vec2d old_mouse_pos_ = Vec2d(0.0, 0.0);
    bool is_mp_valid_ = false;
};

struct Button : public Designable {
    using Designable::Designable;
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

    const char* getText() const { return text_; }
    void setText(const char* text) { text_ = text; }

    bool isPushed() const { return is_pushed_; }
    bool isHovered() const { return is_hovered_; }

   private:
    const char* text_ = "UNDEFINED BTN TEXT";

    bool is_pushed_ = false;
    bool is_hovered_ = false;
    unsigned long long hover_timer_ = 0;
    unsigned long long push_timer_ = 0;
    unsigned long long last_upd_time_ = 0;
    Vec2d known_mouse_pos_ = Vec2d(0.0, 0.0);
    DesignId design_ = DSGN_PANEL_RED;
};

enum class TextAlignment { Left, Center, Right };

struct TextBlock : public Widget {
    using Widget::Widget;

    void draw(plug::TransformStack& stack, plug::RenderTarget& target) override;

    const char* getText() const { return text_; }
    void setText(const char* text) { text_ = text; }

    TextAlignment getAlignment() const { return alignment_; }
    void setAlignment(TextAlignment alignment) { alignment_ = alignment; }

   private:
    const char* text_ = "";
    TextAlignment alignment_ = TextAlignment::Center;
};

struct DragButton : public Button {
    DragButton(Panel& panel);

    void onPush() override;
    void onRelease() override;

   private:
    Panel& panel_;
};

#endif
