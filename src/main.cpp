/**
 * @file main.cpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Main module of the program
 * @version 0.1
 * @date 2023-07-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <math.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>

#include "Impl/Graphics/RenderTarget.h"
#include "Impl/Math/TransformStack.h"
#include "Plug/Math.h"
#include "io/main_io.h"
#include "logger/debug.h"
#include "logger/logger.h"
#include "utils/main_utils.h"
#include "world_timer.h"

#define MAIN

#include "config.h"

int main(const int argc, char** argv) {
    atexit(log_end_program);

    Options options;

    log_init("program_log.html", LOG_THRESHOLD, &errno);
    print_label();

    log_printf(STATUS_REPORTS, "status", "Initializing\n");

    if (argp_parse(&ARG_P, argc, argv, 0, 0, &options) != 0) {
        return EXIT_FAILURE;
    }

    sf::ContextSettings window_settings;
    window_settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(1, 1),
                            "TILT (build from " __DATE__ " " __TIME__ ")",
                            sf::Style::Default, window_settings);

    RenderTarget window_rt(window);

    window.setSize(sf::Vector2u(800, 600));

    Anchor window_anchor(Vec2d(0.0, 0.0), Vec2d(7.0, 7.0),
                         ANCHOR_DEFINITION_SIZE / -2.0,
                         ANCHOR_DEFINITION_SIZE / 2.0);

    Panel root(Anchor(Vec2d(0.0, 0.0), ANCHOR_DEFINITION_SIZE,
                      ANCHOR_DEFINITION_SIZE / -2.0,
                      ANCHOR_DEFINITION_SIZE / 2.0));

    build_gui(root);

    static TransformStack render_stack;

    unsigned long long last_tick_time = WorldTimer::get();
    unsigned long long last_plt_update = WorldTimer::get();
    unsigned long long simulation_time = WorldTimer::get();

    unsigned tick = 0;
    while (window.isOpen()) {
        ++tick;

        unsigned long long new_tick_time = WorldTimer::get();
        double delta_time =
            (double)(new_tick_time - last_tick_time) / 1000000.0;
        last_tick_time = new_tick_time;

        plug::Transform screen_matrix =
            get_screen_transform(window.getSize().x, window.getSize().y);

        render_stack.enter(screen_matrix);

        window_anchor.setSize(Vec2d((double)window.getSize().x / PISKEL_SIZE,
                                    (double)window.getSize().y / PISKEL_SIZE));
        root.onParentUpdate(window_anchor);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            plug::Event* plug_event = parse_event(event, window);
            if (plug_event != nullptr) {
                plug::EHC context = {.stack = render_stack,
                                     .stopped = false,
                                     .overlapped = false};
                root.onEvent(*plug_event, context);
            }
        }

        plug::EHC context = {
            .stack = render_stack, .stopped = false, .overlapped = false};

        root.onEvent(plug::TickEvent(0.02), context);

        Environment::setScreenSize(
            Vec2d(window.getSize().x, window.getSize().y));

        window.clear(sf::Color(0, 20, 0));

        window.setActive(true);

        root.draw(render_stack, window_rt);

        window.display();

        render_stack.leave();
    }

    return errno == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
