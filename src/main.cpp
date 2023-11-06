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

#include "editor_tools/filter.h"
#include "editor_tools/tool.h"
#include "graphics/renderable/buttons/image_scrollbar.h"
#include "graphics/renderable/drawing/tool_selector.h"
#include "graphics/renderable/image_view.h"
#include "graphics/renderable/visuals/fps_meter.h"
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
                            "TILT (build from " __DATE__ __TIME__ ")",
                            sf::Style::Default, window_settings);

    window.setSize(sf::Vector2u(800, 600));

    static Panel root_panel(Vec2d(0.0, 0.0), Vec2d(1.0, 1.0));
    root_panel.set_design(DSGN_PANEL_DEBUG);

    static Panel menu_panel(Vec2d(0.0, -0.25), Vec2d(1.0, 0.5));
    menu_panel.set_design(DSGN_PANEL_SOLID_LIGHT);

    static Panel subwindow_panel(Vec2d(0.0, 0.0), Vec2d(-1.0, 0.5));
    subwindow_panel.set_orderable(true);
    root_panel.add_interactive_child(
        subwindow_panel, Anchor(Vec2d(1.0, -0.25), Vec2d(-1.0, 0.5)));

    static Panel image_panel(Vec2d(0.0, 0.0), Vec2d(13.0, 13.0));
    image_panel.set_design(DSGN_PANEL_SOLID_LIGHT);

    subwindow_panel.add_interactive_child(image_panel);

    static DragButton image_drag(image_panel);

    static ToolPalette tools;
    static FilterPalette filters;

    static ToolSelector selector(tools);
    image_panel.add_interactive_child(selector);

    static ImageView editor(&tools, &filters, Vec2d(0.0, 0.0),
                            Vec2d(12.0, 12.0), "assets/example.bmp");
    image_panel.add_interactive_child(editor);

    static ImageScrollbar horiz_scrollbar(Vec2d(0.0, -(6.0 + SCROLLBAR_WIDTH)),
                                          12.0, editor, SCRLLBR_HORIZONTAL);
    image_panel.add_interactive_child(horiz_scrollbar);

    static ImageScrollbar vert_scrollbar(Vec2d(6.0 + SCROLLBAR_WIDTH, 0.0),
                                         12.0, editor, SCRLLBR_VERTICAL);
    image_panel.add_interactive_child(vert_scrollbar);

    static Panel tool_panel(Vec2d(1.0, 0.0), Vec2d(2.0, 0.5));
    tool_panel.set_design(DSGN_PANEL_SOLID_DARK);
    root_panel.add_interactive_child(
        tool_panel, Anchor(Vec2d(-0.5, -0.25), Vec2d(0.0, 0.5)));

    static Button file_btn(Vec2d(0.5, 0.0), Vec2d(1.0, 1.0), "File");
    static Button edit_btn(Vec2d(1.5, 0.0), Vec2d(1.0, 1.0), "Edit");
    static Button selection_btn(Vec2d(2.5, 0.0), Vec2d(1.0, 1.0), "Select");
    menu_panel.add_interactive_child(file_btn,
                                     Anchor(Vec2d(-0.5, 0.0), Vec2d(0.0, 1.0)));
    menu_panel.add_interactive_child(edit_btn,
                                     Anchor(Vec2d(-0.5, 0.0), Vec2d(0.0, 1.0)));
    menu_panel.add_interactive_child(selection_btn,
                                     Anchor(Vec2d(-0.5, 0.0), Vec2d(0.0, 1.0)));

    static FpsMeter fps_meter(Vec2d(-0.1, -0.1), 1.0);
    menu_panel.add_interactive_child(fps_meter,
                                     Anchor(Vec2d(0.5, 0.5), Vec2d(0.0, 0.0)));

    root_panel.add_interactive_child(menu_panel,
                                     Anchor(Vec2d(0.0, 0.5), Vec2d(1.0, 0.0)));

    static MatrixStack<Mat33d> render_stack(Mat33d(1.0));

    AssetShelf assets;

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

        Mat33d screen_matrix =
            get_screen_matrix(window.getSize().x, window.getSize().y);

        render_stack.push(screen_matrix);

        sf::Event event;
        while (window.pollEvent(event)) {
            root_panel.on_event(
                render_stack,
                (Interaction){
                    .event = &event, .window = &window, .obstructed = false});
            if (event.type == sf::Event::Closed) window.close();
        }

        root_panel.set_size(Vec2d((double)window.getSize().x / 50.0,
                                  (double)window.getSize().y / 50.0));

        window.clear();

        root_panel.render(render_stack, window, assets);

        window.display();

        root_panel.tick();

        render_stack.pop();
    }

    return errno == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
