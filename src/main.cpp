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
#include "graphics/renderable/buttons/render_control.h"
#include "graphics/renderable/ray_tracing_plane.h"
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
                            "Vectorvis (build from " __DATE__ __TIME__ ")",
                            sf::Style::Default, window_settings);

    window.setSize(sf::Vector2u(800, 600));

    MatrixStack<Mat33d> render_stack(Mat33d(1.0));

    Arrow arrow(Vec2d(4.0, M_PI / 2.0));

    RayTracingPlane scene_view(Vec2d(0.0, 0.0), Vec2d(12.0, 10.0), 64, 64);

    for (unsigned id = 0; id < sizeof(LIGHTS) / sizeof(*LIGHTS); ++id) {
        scene_view.add_light(LIGHTS[id]);
    }
    for (unsigned id = 0; id < sizeof(PLANES) / sizeof(*PLANES); ++id) {
        scene_view.add_plane(PLANES[id]);
    }
    for (unsigned id = 0; id < sizeof(SPHERES) / sizeof(*SPHERES); ++id) {
        scene_view.add_sphere(SPHERES[id]);
    }

    Panel menu_panel(Vec2d(8.0, -4.0), Vec2d(4.0, 2.0));
    ProgressResetButton reset_button(scene_view, Vec2d(-0.6, 0.0),
                                     Vec2d(1.0, 0.5), "Reset");
    LightOnOffButton light_button(scene_view, Vec2d(0.6, 0.0), Vec2d(1.0, 0.5),
                                  "Light");

    DragButton menu_move_button(menu_panel);

    menu_panel.add_interactive_child(reset_button);
    menu_panel.add_interactive_child(light_button);
    menu_panel.add_interactive_child(menu_move_button);

    AssetShelf assets = load_assets();

    unsigned tick = 0;
    while (window.isOpen()) {
        ++tick;

        Mat33d screen_matrix =
            get_screen_matrix(window.getSize().x, window.getSize().y);

        render_stack.push(screen_matrix);

        sf::Event event;
        while (window.pollEvent(event)) {
            menu_panel.on_event(
                render_stack,
                (Interaction){
                    .event = &event, .window = &window, .obstructed = false});
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear();

        arrow.set_vector(
            Vec2d(sin(tick / 100.0) * 7.0, cos(tick / 100.0) * 7.0));
        arrow.render(render_stack, window, assets);

        scene_view.process(1);
        scene_view.render(render_stack, window, assets);

        menu_panel.render(render_stack, window, assets);

        window.display();

        render_stack.pop();
    }

    //! ERROR: For some reason, the SFML likes to set errno to 11 (EAGAIN),
    //!     but works just fine... Lets just assume it's some weird
    //!     SFML bug and not the reason why the entirety of my drive will be
    //!     gone within a few launches.

    if (errno == 11) errno = 0;

    return errno == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
