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
#include "graphics/renderable/molecule_view.h"
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

    GasVolume simulation;
    MoleculeView simulation_view(simulation, Vec2d(0.0, 0.0), Vec2d(4.0, 4.0));
    Panel sim_panel(Vec2d(-1.0, 0.0), Vec2d(4.0, 4.0));

    simulation.add_light_molecule(
        LightMolecule(Vec2d(0.0, 0.0), Vec2d(1.0, M_PI)));
    simulation.add_light_molecule(
        LightMolecule(Vec2d(1.0, 0.0), Vec2d(-1.01, M_PI + 0.01)));

    sim_panel.add_child(simulation_view);

    Panel menu_panel(Vec2d(0.0, 0.0), Vec2d(7.0, 5.0));
    ValveControlButton input_button(simulation, VALVE_IN, Vec2d(2.25, 1.5),
                                    Vec2d(1.0, 0.5), "Inflate");
    ValveControlButton output_button(simulation, VALVE_OUT, Vec2d(2.25, 0.5),
                                     Vec2d(1.0, 0.5), "Deflate");

    DragButton menu_move_button(menu_panel);

    menu_panel.add_interactive_child(input_button);
    menu_panel.add_interactive_child(output_button);
    menu_panel.add_interactive_child(menu_move_button);
    menu_panel.add_interactive_child(sim_panel);

    AssetShelf assets;

    unsigned long long last_tick_time = WorldTimer::get();

    unsigned tick = 0;
    while (window.isOpen()) {
        ++tick;

        unsigned long long new_tick_time = WorldTimer::get();
        double delta_time =
            (double)(new_tick_time - last_tick_time) / 1000000.0;
        last_tick_time = new_tick_time;

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

        menu_panel.render(render_stack, window, assets);

        window.display();

        render_stack.pop();

        double phys_dt = 0.0;
        while (phys_dt < delta_time * PHYS_SIM_SPEED) {
            simulation.tick(PHYS_TIME_STEP);
            phys_dt += PHYS_TIME_STEP;
        }
    }

    //! ERROR: For some reason, the SFML likes to set errno to 11 (EAGAIN),
    //!     but works just fine... Lets just assume it's some weird
    //!     SFML bug and not the reason why the entirety of my drive will be
    //!     gone within a few launches.

    if (errno == 11) errno = 0;

    return errno == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
