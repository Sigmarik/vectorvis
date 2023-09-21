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

struct Interaction {
    sf::Event* event = NULL;
    sf::Window* window = NULL;
    bool obstructed = false;
};

struct Interactive : public Renderable {
    virtual ~Interactive() = default;

    virtual void on_event(MatrixStack<Mat33d>& stack, Interaction interaction);

    Vec2d get_center() const;
    Vec2d get_size() const;
    void move(const Vec2d& new_center);
    void resize(const Vec2d& new_size);

    bool is_under(const MatrixStack<Mat33d>& stack, const Vec2d& screen_pos);

   protected:
    Vec2d center_ = Vec2d(), size_ = Vec2d(1.0, 1.0);
    Vec2d mouse_position_ = Vec2d(0.0, 0.0);
};

#endif
