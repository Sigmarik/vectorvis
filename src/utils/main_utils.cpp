#include "main_utils.h"

#include <stdlib.h>

#include "lib/logger/logger.h"
#include "src/config.h"

void print_label() {
    puts(PROGRAM_NAME);
    printf("Build from\n%s %s\n", __DATE__, __TIME__);
    log_printf(ABSOLUTE_IMPORTANCE, "build info", "Build from %s %s.\n",
               __DATE__, __TIME__);
}

plug::Transform get_screen_transform(unsigned size_x, unsigned size_y) {
    return plug::Transform(Vec2d(0.5, 0.5),
                           Vec2d(50.0 / size_x, -50.0 / size_y));
}

static plug::MouseButton sf_mbtn_to_plug_mbtn(sf::Mouse::Button button) {
    switch (button) {
        case sf::Mouse::Button::Left:
            return plug::MouseButton::Left;
        case sf::Mouse::Button::Right:
            return plug::MouseButton::Right;
        case sf::Mouse::Button::Middle:
            return plug::MouseButton::Middle;
        case sf::Mouse::Button::XButton1:
            return plug::MouseButton::XButton1;
        case sf::Mouse::Button::XButton2:
            return plug::MouseButton::XButton2;
        default:
            break;
    }

    return plug::MouseButton::Left;
}

static plug::KeyCode sf_key_to_plug_key(sf::Keyboard key) {
    return plug::KeyCode::F;  // TODO: Implement
}

plug::Event* parse_event(const sf::Event& event, sf::Window& window) {
    static plug::MouseMoveEvent mouse_move(Vec2d(0.0, 0.0), false, false,
                                           false);
    static plug::MousePressedEvent mouse_pressed(
        plug::MouseButton::Left, Vec2d(0.0, 0.0), false, false, false);
    static plug::MouseReleasedEvent mouse_released(
        plug::MouseButton::Left, Vec2d(0.0, 0.0), false, false, false);
    static plug::KeyboardPressedEvent keyboard_press(plug::KeyCode::F, false,
                                                     false, false);
    static plug::KeyboardReleasedEvent keyboard_release(plug::KeyCode::F, false,
                                                        false, false);

    static bool shift = false, control = false, alt = false;

    switch (event.type) {
        case sf::Event::EventType::MouseMoved: {
            mouse_move.pos = Vec2d((double)sf::Mouse::getPosition(window).x /
                                       (double)window.getSize().x,
                                   (double)sf::Mouse::getPosition(window).y /
                                       (double)window.getSize().y);
            mouse_move.shift = shift;
            mouse_move.ctrl = control;
            mouse_move.alt = alt;
            return &mouse_move;
        } break;
        case sf::Event::EventType::MouseButtonPressed: {
            mouse_pressed.button_id =
                sf_mbtn_to_plug_mbtn(event.mouseButton.button);
            mouse_pressed.pos = Vec2d((double)sf::Mouse::getPosition(window).x /
                                          (double)window.getSize().x,
                                      (double)sf::Mouse::getPosition(window).y /
                                          (double)window.getSize().y);
            mouse_pressed.shift = shift;
            mouse_pressed.ctrl = control;
            mouse_pressed.alt = alt;
            return &mouse_pressed;
        } break;
        case sf::Event::EventType::MouseButtonReleased: {
            mouse_released.button_id =
                sf_mbtn_to_plug_mbtn(event.mouseButton.button);
            mouse_released.pos =
                Vec2d((double)sf::Mouse::getPosition(window).x /
                          (double)window.getSize().x,
                      (double)sf::Mouse::getPosition(window).y /
                          (double)window.getSize().y);
            mouse_released.shift = shift;
            mouse_released.ctrl = control;
            mouse_released.alt = alt;
            return &mouse_released;
        } break;
        // TODO: Translate keyboard events
        default:
            break;
    }

    return nullptr;
}
