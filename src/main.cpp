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

#include "graphics/renderable/arrow.h"
#include "graphics/renderable/shadetangle.h"
#include "graphics/shaders/rt_scene.h"
#include "io/main_io.h"
#include "logger/debug.h"
#include "logger/logger.h"
#include "utils/main_utils.h"

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
                            "Vectorviz (build from " __DATE__ __TIME__ ")",
                            sf::Style::Default, window_settings);

    window.setSize(sf::Vector2u(800, 600));

    MatrixStack<Mat33d> render_stack(Mat33d(1.0));

    Arrow arrow(Vec2d(4.0, M_PI / 2.0));
    Shadetangle scene_view(rt_example, Vec2d(0.0, 0.0), Vec2d(12.0, 10.0), 256,
                           256);

    unsigned tick = 0;
    while (window.isOpen()) {
        ++tick;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        Mat33d screen_matrix =
            get_screen_matrix(window.getSize().x, window.getSize().y);

        render_stack.push(screen_matrix);

        window.clear();

        arrow.set_vector(
            Vec2d(sin(tick / 100.0) * 7.0, cos(tick / 100.0) * 7.0));
        arrow.render(render_stack, window);

        scene_view.render(render_stack, window);

        window.display();

        render_stack.pop();
    }

    //! ERROR: For some reason, the constructor above sets errno to 11 (EAGAIN),
    //!     but works just fine afterwards... Lets just hope it's some weird
    //!     SFML bug and not the reason why the entirety of my drive will be
    //!     gone within a few launches.

    return errno == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
