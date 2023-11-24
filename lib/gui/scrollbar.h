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

#include "gui.h"

static const double SCROLLBAR_WIDTH = 0.25;

struct Scrollbar;
struct ScrollbarBar;

static const double SCROLLBAR_HANDLE_HEIGHT = 0.1;

enum ScrollbarType {
    SCRLLBR_HORIZONTAL,
    SCRLLBR_VERTICAL,
};

enum ScrollbarButtonType {
    SCRLL_UP,
    SCRLL_DOWN,
};

/**
 * @brief Draggable scrollbar handle
 *
 */
struct ScrollbarHandle : public Button {
    ScrollbarHandle(ScrollbarBar& bar);

    bool on_mouse_move(const Vec2d& mouse_pos, TransformStack& stack) override;

    void on_push() override;

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
struct ScrollbarBar : public Button {
    ScrollbarBar(Scrollbar& bar);

    void on_push() override;

    double get_percentage() const;
    void set_percentage(double percentage);

    ScrollbarType get_type() const { return type_; }

   private:
    ScrollbarBar(const ScrollbarBar& bar) = default;
    ScrollbarBar& operator=(const ScrollbarBar& bar) = default;

    ScrollbarHandle handle_;
    Scrollbar& bar_;
    ScrollbarType type_;
};

/**
 * @brief Little button at the end of the scrollbar used to shift scrollbar
 * handle up or down with clicks
 *
 */
struct ScrollbarControlBtn : public Button {
    ScrollbarControlBtn(Scrollbar& bar, ScrollbarButtonType type);

    void on_push() override;

    bool on_tick(double delta_time) override;

   private:
    ScrollbarControlBtn(const ScrollbarControlBtn& btn) = default;
    ScrollbarControlBtn& operator=(const ScrollbarControlBtn& btn) = default;

    Scrollbar& bar_;

    ScrollbarButtonType type_ = SCRLL_UP;

    unsigned long long push_time_ = 0;
    unsigned long long update_time_ = 0;
};

/**
 * @brief Scrollbar GUI element
 *
 */
struct Scrollbar : Panel {
    Scrollbar(Vec2d center, double length,
              ScrollbarType type = SCRLLBR_VERTICAL);

    virtual void on_update(double percentage) {}

    double get_percentage() const;
    void set_percentage(double percentage);

    ScrollbarType get_type() const { return type_; }

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
