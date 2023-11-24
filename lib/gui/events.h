/**
 * @file events.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Event classes
 * @version 0.1
 * @date 2023-11-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef EVENT_H
#define EVENT_H

#include "geometry/vector.hpp"
#include "stdlib.h"

struct Event {
    Event(size_t type) : m_type(type) {}
    size_t getType() const { return m_type; }

   private:
    const size_t m_type;
};

enum EventType {
    Tick = 0,
    MouseMove = 1,
    MousePressed = 2,
    MouseReleased = 3,
    KeyboardPressed = 4,
    KeyboardReleased = 5,
};

typedef sf::Mouse::Button MouseButton;
typedef sf::Keyboard::Key KeyboardKey;

struct MouseMoveEvent : public Event {
    bool shift = false, ctrl = false, alt = false;
    Vec2d pos = Vec2d(0.0, 0.0);
} struct MousePressedEvent : public Event {
    MouseButton button_id = 0;
    bool shift = false, ctrl = false, alt = false;
    Vec2d pos = Vec2d(0.0, 0.0);
} struct MouseReleasedEvent : public Event {
    MouseButton button_id = 0;
    bool shift = false, ctrl = false, alt = false;
    Vec2d pos = Vec2d(0.0, 0.0);
} struct KeyboardPressedEvent : public Event {
    KeyCode key_id = 0;
    bool shift = false, ctrl = false, alt = false;
    Vec2d pos = Vec2d(0.0, 0.0);
} struct KeyboardReleasedEvent : public Event {
    KeyCode key_id = 0;
    bool shift = false, ctrl = false, alt = false;
    Vec2d pos = Vec2d(0.0, 0.0, 0.0);
} struct TickEvent : public Event {
    double delta_time = 0.0;
}

#endif
