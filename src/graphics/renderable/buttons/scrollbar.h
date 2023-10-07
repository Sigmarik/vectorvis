/**
 * @file scrollbar.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Scrollbar GUI element
 * @version 0.1
 * @date 2023-10-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include "src/graphics/renderable/gui.h"

static const double SCROLLBAR_WIDTH = 0.25;

struct Scrollbar;
struct ScrollbarBar;

static const double SCROLLBAR_HANDLE_HEIGHT = 0.1;

/**
 * @brief Draggable scrollbar handle
 *
 */
struct ScrollbarHandle : public Button {
    ScrollbarHandle(ScrollbarBar& bar);

    void on_event(MatrixStack<Mat33d>& stack, Interaction interaction) override;

    void on_push(Interaction interaction) override;

   private:
    ScrollbarHandle(const ScrollbarHandle& handle) = default;
    ScrollbarHandle& operator=(const ScrollbarHandle& handle) = default;

    ScrollbarBar& bar_;
    Vec2d prev_mouse_pos_ = Vec2d(0.0, 0.0);
};

/**
 * @brief Central part of the scrollbar, the one which the handle slides in
 *
 */
struct ScrollbarBar : public Panel {
    ScrollbarBar(Scrollbar& bar);

    void on_event(MatrixStack<Mat33d>& stack, Interaction interaction) override;

    double get_percentage() const;
    void set_percentage(double percentage);

   private:
    ScrollbarBar(const ScrollbarBar& bar) = default;
    ScrollbarBar& operator=(const ScrollbarBar& bar) = default;

    ScrollbarHandle handle_;
    Scrollbar& bar_;
};

enum ScrollbarButtonType {
    SCRLL_UP,
    SCRLL_DOWN,
};

/**
 * @brief Little button at the end of the scrollbar used to shift scrollbar
 * handle up or down with clicks
 *
 */
struct ScrollbarControlBtn : public Button {
    ScrollbarControlBtn(Scrollbar& bar, ScrollbarButtonType type);

    void on_push(Interaction interaction) override;

    void tick() override;

   private:
    ScrollbarControlBtn(const ScrollbarControlBtn& btn) = default;
    ScrollbarControlBtn& operator=(const ScrollbarControlBtn& btn) = default;

    Scrollbar& bar_;

    ScrollbarButtonType type_ = SCRLL_UP;

    unsigned long long push_time_ = 0;
    unsigned long long update_time_ = 0;
};

enum ScrollbarType {
    SCRLLBR_HORIZONTAL,
    SCRLLBR_VERTICAL,
};

/**
 * @brief Scrollbar GUI element
 *
 */
struct Scrollbar : Panel {
    Scrollbar(Vec2d center, double length,
              ScrollbarType type = SCRLLBR_HORIZONTAL);

    virtual void on_update(double percentage) {}

    double get_percentage() const;
    void set_percentage(double percentage);

    void apply_anchor(const Vec2d& parent_size) override;

   protected:
    ScrollbarType type_ = SCRLLBR_VERTICAL;

   private:
    Scrollbar(const Scrollbar& bar) = default;
    Scrollbar& operator=(const Scrollbar& bar) = default;

    ScrollbarControlBtn up_btn_;
    ScrollbarControlBtn down_btn_;
    ScrollbarBar bar_;
};

#endif
