/**
 * @file interactive.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Interactive graphical element
 * @version 0.1
 * @date 2023-09-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>

#include "renderable.h"

struct WorldTimer final {
    static unsigned long long get();

   private:
    WorldTimer();
    WorldTimer& operator=(const WorldTimer& timer);
    sf::Clock clock_;
};

struct Anchor {
    Anchor() : center_(0.0, 0.0), size_(0.0, 0.0) {}
    Anchor(Vec2d center, Vec2d size) : center_(center), size_(size) {}
    Anchor(const Anchor& anchor) = default;

    Anchor& operator=(const Anchor& anchor) = default;

    Mat33d get_matrix(Vec2d parent_size);

    Vec2d get_center() { return center_; }
    Vec2d get_size() { return size_; }

   private:
    Vec2d center_;
    Vec2d size_;
};

struct Interaction {
    sf::Event* event = NULL;
    sf::Window* window = NULL;
    bool obstructed = false;
};

enum Corner {
    TOP_LEFT = 0,
    TOP_RIGHT = 1,
    BOTTOM_LEFT = 2,
    BOTTOM_RIGHT = 3,
};

struct Interactive : public Renderable {
    Interactive(const Vec2d& center, const Vec2d& size);
    virtual ~Interactive() = default;

    virtual void on_event(MatrixStack<Mat33d>& stack, Interaction interaction);

    Vec2d get_center() const { return center_; }
    Vec2d get_size() const { return size_; }
    void set_center(const Vec2d& new_center) {
        center_ = new_center;
        vis_center_ = center_;
    }
    void set_size(const Vec2d& new_size) {
        size_ = new_size;
        vis_size_ = size_;
    }

    void set_anchor(const Anchor& anchor) { anchor_ = anchor; }
    Anchor get_anchor() const { return anchor_; }

    void apply_anchor(const Vec2d& parent_size);

    bool is_under(const MatrixStack<Mat33d>& stack, const Vec2d& screen_pos);

   protected:
    Vec2d get_corner(Corner corner, const MatrixStack<Mat33d>& stack);

   protected:
    Vec2d center_ = Vec2d(), size_ = Vec2d(1.0, 1.0);
    Vec2d vis_center_ = Vec2d(), vis_size_ = Vec2d(1.0, 1.0);
    Vec2d mouse_position_ = Vec2d(0.0, 0.0);

    Anchor anchor_ = Anchor();
};

#endif
