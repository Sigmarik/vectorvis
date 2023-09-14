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

#include "interactive.h"

struct Panel : public Interactive {
    explicit Panel(const Vec2d& center, const Vec2d& size);

    void render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                const AssetShelf& assets) override;

    void on_event(MatrixStack<Mat33d>& stack, Interaction interaction) override;

    void add_child(Renderable& child);
    void add_interactive_child(Interactive& child);

    bool is_movable() const;
    void set_movable(bool movable);

   private:
    Mat33d get_matrix();

    Stack<Renderable*> children_;
    Stack<Interactive*> interactive_children_;

    Vec2d last_cursor_position_ = Vec2d();
    bool movable_ = false;
};

static const Vec3d DEFAULT_BUTTON_COLOR(0.24, 0.25, 0.33);

struct Button : public Interactive {
    Button(const Vec2d& center, const Vec2d& size, const char* text = "Button",
           const Vec3d& color = DEFAULT_BUTTON_COLOR);
    Button(const Button& button) = default;
    virtual ~Button() = default;

    Button& operator=(const Button& button) = default;

    void render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                const AssetShelf& assets) override;

    void on_event(MatrixStack<Mat33d>& stack, Interaction interaction) override;

    virtual void on_push(Interaction interaction);
    virtual void on_release(Interaction interaction);

    void set_text(const char* text);
    void set_color(const Vec3d& color);

    bool is_pushed();

   private:
    const char* text_;
    Vec3d color_;

    bool is_pushed_ = false;
    double color_amplifier_ = 1.0;
};

struct DragButton : public Button {
    DragButton(Panel& panel);

    void on_push(Interaction interaction) override;
    void on_release(Interaction interaction) override;

   private:
    Panel& panel_;
};

#endif
