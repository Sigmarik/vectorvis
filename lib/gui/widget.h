/**
 * @file widget.cpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Widget class
 * @version 0.1
 * @date 2023-11-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef WIDGET_H
#define WIDGET_H

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>

#include "data_structures/stack.h"
#include "layout_box.h"
#include "renderable.h"

/*
+ bool onMouseMove(const Vec &mouse_position, StackTransform &stack)
+ bool onMouseButtonPressed(const Vec &mouse_position, enum MouseButton
button_id, StackTransform &stack)
+ bool onMouseButtonReleased(const Vec &mouse_position, enum MouseButton
button_id, StackTransform &stack)
+ bool onKeyPressed(enum KeyCode key_id)
+ bool onKeyReleased(enum KeyCode key_id)
+ bool onTick(double delta_time)
+ void onParentUpdate(Layout &parent_layout)
+ void draw(sf::RenderTarget &result, StackTransform &stack)
+ LayoutBox &getLayoutBox(void)
+ const LayoutBox &getLayoutBox(void) const
+ void setLayoutBox(const LayoutBox &new_layoutBox)
*/

typedef sf::Mouse::Button MouseButton;
typedef sf::Keyboard::Key KeyboardKey;

struct Widget : Renderable {
    Widget(const LayoutBox& box);
    Widget(const Widget& widget);
    Widget& operator=(const Widget& widget);
    virtual ~Widget();

    void render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                const AssetShelf& assets) override;

    virtual bool on_mouse_move(const Vec2d& mouse_pos, TransformStack& stack);
    virtual bool on_mouse_button_pressed(const Vec2d& mouse_pos,
                                         MouseButton button,
                                         TransformStack& stack);
    virtual bool on_mouse_button_released(const Vec2d& mouse_pos,
                                          MouseButton button,
                                          TransformStack& stack);
    virtual bool on_key_pressed(KeyboardKey key);
    virtual bool on_key_released(KeyboardKey key);
    virtual bool on_tick(double delta_time);

    virtual bool on_any_event(const sf::Event& event, const sf::Window& window,
                              TransformStack& stack);

    virtual void on_parent_update(const LayoutBox& parent_box);

    virtual Mat33d get_local_space() const;

    LayoutBox& get_layout_box() { return *box_; }
    const LayoutBox& get_layout_box() const { return *box_; }
    void set_layout_box(const LayoutBox& box) {
        delete box_;
        box_ = box.clone();
    }

    void add_child(Widget& widget);

    const Stack<Widget*>& get_children() const { return children_; }
    Widget* get_focused_child() const {
        return children_.size() > 0 ? children_.top() : nullptr;
    }

    bool focused() const { return focused_; }

    void focus(Widget& widget);

    void dispatch_input(TransformStack& stack, const sf::Event& event,
                        const sf::Window& window);

   protected:
    Vec2d get_corner(Corner corner, TransformStack stack) const;
    virtual bool is_under(TransformStack& stack, const Vec2d& screen_pos) const;

   private:
    Stack<Widget*> children_ = Stack<Widget*>();
    LayoutBox* box_;
    bool focused_ = false;
};

#endif
