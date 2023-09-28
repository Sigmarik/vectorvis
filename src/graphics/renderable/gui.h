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

    void add_child(Renderable& child, Anchor anchor = Anchor());
    void add_interactive_child(Interactive& child, Anchor anchor = Anchor());

    void focus_child(const Interactive& child);

    bool is_movable() const;
    void set_movable(bool movable);

    bool is_orderable() const { return orderable_; }
    void set_orderable(bool orderable) { orderable_ = false; }

    void set_design(DesignId id) { design_ = id; }
    DesignId get_design() const { return design_; }

   private:
    Mat33d get_matrix();

    void fill_shader_parameters(MatrixStack<Mat33d>& stack,
                                sf::RenderTarget& target,
                                const AssetShelf& assets);

    Stack<Renderable*> children_;
    Stack<Interactive*> interactive_children_;

    Vec2d last_cursor_position_ = Vec2d();
    DesignId design_ = DSGN_PANEL_DEFAULT;
    bool movable_ = false;
    bool orderable_ = false;
};

static const Vec3d DEFAULT_BUTTON_COLOR(0.24, 0.25, 0.33);

struct Button : public Interactive {
    Button(const Vec2d& center, const Vec2d& size, const char* text = "Button");
    Button(const Button& button) = default;
    virtual ~Button() = default;

    Button& operator=(const Button& button) = default;

    void render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                const AssetShelf& assets) override;

    void on_event(MatrixStack<Mat33d>& stack, Interaction interaction) override;

    virtual void on_push(Interaction interaction);
    virtual void on_release(Interaction interaction);

    void set_text(const char* text) { text_ = text; }

    bool is_pushed();

    void set_design(DesignId id) { design_ = id; }
    DesignId get_design() { return design_; }

   private:
    void update_timers();
    void fill_shader_parameters(MatrixStack<Mat33d>& stack,
                                sf::RenderTarget& target,
                                const AssetShelf& assets);

    const char* text_;

    bool is_pushed_ = false;
    bool is_hovered_ = false;
    unsigned long long hover_timer_ = 0.0;
    unsigned long long push_timer_ = 0.0;
    unsigned long long last_upd_time_ = 0.0;
    DesignId design_ = DSGN_BUTTON_DEFAULT;
};

struct DragButton : public Button {
    DragButton(Panel& panel);

    void on_push(Interaction interaction) override;
    void on_release(Interaction interaction) override;

   private:
    Panel& panel_;
};

#endif
