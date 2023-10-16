/**
 * @file mock_tool.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Tool class mockup
 * @version 0.1
 * @date 2023-10-11
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef TOOL_H
#define TOOL_H

#include "src/graphics/renderable/image_view.h"

enum ButtonState {
    BTN_DOWN,
    BTN_UP,
};

struct Tool {
    virtual ~Tool() = default;

    virtual void render(const MatrixStack<Mat33d>& stack,
                        sf::RenderTarget& target, const ImageView& view){};

    virtual void on_main(ButtonState state, Vec2d pos, ImageView& view) {}
    virtual void on_sec(ButtonState state, Vec2d pos, ImageView& view) {}

    virtual void on_modifier1(ButtonState state, Vec2d pos, ImageView& view) {}
    virtual void on_modifier2(ButtonState state, Vec2d pos, ImageView& view) {}
    virtual void on_modifier3(ButtonState state, Vec2d pos, ImageView& view) {}

    virtual void on_move(Vec2d pos, ImageView& view) {}

    virtual void cancel() { active_ = false; }
    virtual void confirm(ImageView& image_view) {}

   protected:
    bool active_ = false;
    unsigned thickness_ = 3;
};

struct ToolPallet {
    ToolPallet() = default;
    ~ToolPallet() = default;

    Tool* get_tool() const { return active_tool_; }
    void set_tool(Tool* tool) { active_tool_ = tool; }

    sf::Color get_bg_color() const { return bg_color_; }
    void set_bg_color(sf::Color color) { bg_color_ = color; }

    sf::Color get_fg_color() const { return fg_color_; }
    void set_fg_color(sf::Color color) { fg_color_ = color; }

   private:
    Tool* active_tool_ = nullptr;
    sf::Color fg_color_ = sf::Color::Red;
    sf::Color bg_color_ = sf::Color::Black;
};

struct LineTool : Tool {
    void render(const MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                const ImageView& view) override;

    void on_main(ButtonState state, Vec2d pos, ImageView& view) override;
    void on_move(Vec2d pos, ImageView& view) override;

    void confirm(ImageView& view) override;

   private:
    Vec2d start_pos_ = Vec2d();
    Vec2d end_pos_ = Vec2d();
};

struct CircleTool : Tool {
    void render(const MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                const ImageView& view) override;

    void on_main(ButtonState state, Vec2d pos, ImageView& view) override;
    void on_modifier1(ButtonState state, Vec2d pos, ImageView& view) override;
    void on_move(Vec2d pos, ImageView& view) override;

    void confirm(ImageView& view) override;

   private:
    Vec2d start_pos_ = Vec2d();
    Vec2d end_pos_ = Vec2d();
    bool round_ = false;
};

struct PencilTool : Tool {
    void render(const MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                const ImageView& view) override;

    void on_main(ButtonState state, Vec2d pos, ImageView& view) override;
    void on_sec(ButtonState state, Vec2d pos, ImageView& view) override;
    void on_move(Vec2d pos, ImageView& view) override;

    void confirm(ImageView& view) override;

   private:
    Vec2d start_pos_ = Vec2d();
    Vec2d end_pos_ = Vec2d();
    bool background_ = false;
    bool modified_ = false;
};

struct StripTool : Tool {
    void render(const MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                const ImageView& view) override;

    void on_main(ButtonState state, Vec2d pos, ImageView& view) override;
    void on_sec(ButtonState state, Vec2d pos, ImageView& view) override;
    void on_move(Vec2d pos, ImageView& view) override;

    void cancel() override;

    void confirm(ImageView& view) override;

   private:
    Stack<Vec2d> points_;
    Vec2d current_ = Vec2d(0.0, 0.0);
};

#endif
