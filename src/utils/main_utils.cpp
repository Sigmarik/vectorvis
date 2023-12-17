#include "main_utils.h"

#include <stdlib.h>

#include "extern/filter_palette.h"
#include "extern/tool_palette.h"
#include "gui/buttons/color_picker.h"
#include "gui/buttons/filter_selector_btn.h"
#include "gui/buttons/tool_selector_btn.h"
#include "gui/dropdown_list.h"
#include "gui/scrollbar.h"
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
                           Vec2d(PISKEL_SIZE / size_x, -PISKEL_SIZE / size_y));
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

#define CONVERT_KEY(key)    \
    case sf::Keyboard::key: \
        return plug::KeyCode::key

static plug::KeyCode sf_key_to_plug_key(sf::Keyboard::Key key) {
    switch (key) {
        CONVERT_KEY(A);
        CONVERT_KEY(B);
        CONVERT_KEY(C);
        CONVERT_KEY(D);
        CONVERT_KEY(E);
        CONVERT_KEY(F);
        CONVERT_KEY(G);
        CONVERT_KEY(H);
        CONVERT_KEY(I);
        CONVERT_KEY(J);
        CONVERT_KEY(K);
        CONVERT_KEY(L);
        CONVERT_KEY(M);
        CONVERT_KEY(N);
        CONVERT_KEY(O);
        CONVERT_KEY(P);
        CONVERT_KEY(Q);
        CONVERT_KEY(R);
        CONVERT_KEY(S);
        CONVERT_KEY(T);
        CONVERT_KEY(U);
        CONVERT_KEY(V);
        CONVERT_KEY(W);
        CONVERT_KEY(X);
        CONVERT_KEY(Y);
        CONVERT_KEY(Z);
        CONVERT_KEY(Num0);
        CONVERT_KEY(Num1);
        CONVERT_KEY(Num2);
        CONVERT_KEY(Num3);
        CONVERT_KEY(Num4);
        CONVERT_KEY(Num5);
        CONVERT_KEY(Num6);
        CONVERT_KEY(Num7);
        CONVERT_KEY(Num8);
        CONVERT_KEY(Num9);
        CONVERT_KEY(Escape);
        CONVERT_KEY(LControl);
        CONVERT_KEY(LShift);
        CONVERT_KEY(LAlt);
        CONVERT_KEY(LSystem);
        CONVERT_KEY(RControl);
        CONVERT_KEY(RShift);
        CONVERT_KEY(RAlt);
        CONVERT_KEY(RSystem);
        CONVERT_KEY(Menu);
        CONVERT_KEY(LBracket);
        CONVERT_KEY(RBracket);
        CONVERT_KEY(Semicolon);
        CONVERT_KEY(Comma);
        CONVERT_KEY(Period);
        CONVERT_KEY(Quote);
        CONVERT_KEY(Slash);
        CONVERT_KEY(Backslash);
        CONVERT_KEY(Tilde);
        CONVERT_KEY(Equal);
        CONVERT_KEY(Hyphen);
        CONVERT_KEY(Space);
        CONVERT_KEY(Enter);
        CONVERT_KEY(Backspace);
        CONVERT_KEY(Tab);
        CONVERT_KEY(PageUp);
        CONVERT_KEY(PageDown);
        CONVERT_KEY(End);
        CONVERT_KEY(Home);
        CONVERT_KEY(Insert);
        CONVERT_KEY(Delete);
        CONVERT_KEY(Add);
        CONVERT_KEY(Subtract);
        CONVERT_KEY(Multiply);
        CONVERT_KEY(Divide);
        CONVERT_KEY(Left);
        CONVERT_KEY(Right);
        CONVERT_KEY(Up);
        CONVERT_KEY(Down);
        CONVERT_KEY(Numpad0);
        CONVERT_KEY(Numpad1);
        CONVERT_KEY(Numpad2);
        CONVERT_KEY(Numpad3);
        CONVERT_KEY(Numpad4);
        CONVERT_KEY(Numpad5);
        CONVERT_KEY(Numpad6);
        CONVERT_KEY(Numpad7);
        CONVERT_KEY(Numpad8);
        CONVERT_KEY(Numpad9);
        CONVERT_KEY(F1);
        CONVERT_KEY(F2);
        CONVERT_KEY(F3);
        CONVERT_KEY(F4);
        CONVERT_KEY(F5);
        CONVERT_KEY(F6);
        CONVERT_KEY(F7);
        CONVERT_KEY(F8);
        CONVERT_KEY(F9);
        CONVERT_KEY(F10);
        CONVERT_KEY(F11);
        CONVERT_KEY(F12);
        CONVERT_KEY(F13);
        CONVERT_KEY(F14);
        CONVERT_KEY(F15);
        CONVERT_KEY(Pause);

        CONVERT_KEY(KeyCount);
        CONVERT_KEY(Unknown);

        default:
            return plug::KeyCode::Unknown;
    }
}

#undef CONVERT_KEY

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
        case sf::Event::EventType::KeyPressed: {
            keyboard_press.key_id = sf_key_to_plug_key(event.key.code);
            keyboard_press.shift = shift;
            keyboard_press.ctrl = control;
            keyboard_press.alt = alt;
            return &keyboard_press;
        } break;
        case sf::Event::EventType::KeyReleased: {
            keyboard_press.key_id = sf_key_to_plug_key(event.key.code);
            keyboard_press.shift = shift;
            keyboard_press.ctrl = control;
            keyboard_press.alt = alt;
            return &keyboard_press;
        } break;
        default:
            break;
    }

    return nullptr;
}

static void construct_tool_list(Panel& holder) {
    plug::LayoutBox& box = holder.getLayoutBox();

    box.setSize(box.getSize() + Vec2d(0.0, 0.5 * (double)ToolPalette::size()));
    box.setPosition(box.getPosition() +
                    Vec2d(0.0, -0.25 * (double)ToolPalette::size()));

    for (size_t id = 0; id < ToolPalette::size(); ++id) {
        const plug::PluginData* data = ToolPalette::getTool(id).getPluginData();

        double height =
            0.25 * (double)ToolPalette::size() - 0.5 * (double)id - 0.25;

        ToolSelectionButton* button = new ToolSelectionButton(
            id, Anchor(Vec2d(0.0, height),
                       Vec2d(holder.getLayoutBox().getSize().x, 0.5),
                       Vec2d(0.0, 0.5), Vec2d(0.0, 0.5)));

        button->setText(data != nullptr ? data->getName() : "Unnamed Tool");

        holder.addChild(*button);
    }
}

static void construct_filter_list(Panel& holder, CanvasView& view) {
    plug::LayoutBox& box = holder.getLayoutBox();

    box.setSize(box.getSize() +
                Vec2d(0.0, 0.5 * (double)FilterPalette::size()));
    box.setPosition(box.getPosition() +
                    Vec2d(0.0, -0.25 * (double)FilterPalette::size()));

    for (size_t id = 0; id < FilterPalette::size(); ++id) {
        const plug::PluginData* data =
            FilterPalette::getFilter(id).getPluginData();

        double height =
            0.25 * (double)FilterPalette::size() - 0.5 * (double)id - 0.25;

        FilterSelectionButton* button = new FilterSelectionButton(
            id, view,
            Anchor(Vec2d(0.0, height),
                   Vec2d(holder.getLayoutBox().getSize().x, 0.5),
                   Vec2d(0.0, 0.0), Vec2d(0.0, 0.0)));

        button->setText(data != nullptr ? data->getName() : "Unnamed Filter");

        holder.addChild(*button);
    }

    // holder.onParentUpdate(box);
}

void build_gui(Panel& root) {
    static Panel top_menu(Anchor(Vec2d(0.0, -0.25),
                                 Vec2d(ANCHOR_DEFINITION_SIZE.x, 0.5),
                                 ANCHOR_DEFINITION_SIZE * Vec2d(-0.5, 0.5),
                                 ANCHOR_DEFINITION_SIZE * Vec2d(0.5, 0.5)));

    //* CANVAS SETUP
    static ColorPalette palette = ColorPalette();
    static Canvas canvas("assets/example.bmp");

    for (size_t id = 0; id < ToolPalette::size(); ++id) {
        plug::Tool& tool = ToolPalette::getTool(id);
        tool.setActiveCanvas(canvas);
        tool.setColorPalette(palette);
    }

    static Panel canvas_frame(Anchor(Vec2d(0.0, 0.0), Vec2d(13.0, 13.0),
                                     Vec2d(0.0, 0.0), Vec2d(0.0, 0.0)));
    static CanvasView canvas_view(Anchor(Vec2d(0.0, 0.0), Vec2d(12.0, 12.0),
                                         Vec2d(0.0, 0.0), Vec2d(0.0, 0.0)),
                                  canvas);
    canvas_frame.addChild(canvas_view);
    static DragButton canvas_drag(canvas_frame);
    canvas_frame.addChild(canvas_drag);

    //* SIDE PANEL
    static Panel side_panel(
        Anchor(Vec2d(-1.5, 0.0), Vec2d(3.0, ANCHOR_DEFINITION_SIZE.y - 0.5),
               ANCHOR_DEFINITION_SIZE * Vec2d(0.5, -0.5),
               ANCHOR_DEFINITION_SIZE * Vec2d(0.5, 0.5) + Vec2d(0.0, -0.5)));

    static ColorPicker fg_picker(
        palette, PickerType::Foreground,
        Anchor(Vec2d(1.0, -1.0), Vec2d(1.0, 0.5),
               ANCHOR_DEFINITION_SIZE * Vec2d(-0.5, 0.5),
               ANCHOR_DEFINITION_SIZE * Vec2d(-0.5, 0.5)));

    static ColorPicker bg_picker(
        palette, PickerType::Background,
        Anchor(Vec2d(1.0, -2.0), Vec2d(1.0, 0.5),
               ANCHOR_DEFINITION_SIZE * Vec2d(-0.5, 0.5),
               ANCHOR_DEFINITION_SIZE * Vec2d(-0.5, 0.5)));

    static TextBlock fg_descriptor(
        Anchor(Vec2d(2.0, -1.0), Vec2d(1.0, 0.5),
               ANCHOR_DEFINITION_SIZE * Vec2d(-0.5, 0.5),
               ANCHOR_DEFINITION_SIZE * Vec2d(-0.5, 0.5)));

    fg_descriptor.setText("FRG");

    static TextBlock bg_descriptor(
        Anchor(Vec2d(2.0, -2.0), Vec2d(1.0, 0.5),
               ANCHOR_DEFINITION_SIZE * Vec2d(-0.5, 0.5),
               ANCHOR_DEFINITION_SIZE * Vec2d(-0.5, 0.5)));

    bg_descriptor.setText("BKG");

    side_panel.addChild(bg_picker);
    side_panel.addChild(bg_descriptor);

    side_panel.addChild(fg_descriptor);
    side_panel.addChild(fg_picker);

    //* TOOL AND FILTER LISTS SETUP
    static Panel tool_list(Anchor(Vec2d(0.8, -0.25), Vec2d(3.0, 0.0),
                                  Vec2d(0.0, 0.0), Vec2d(0.0, 0.0)));
    construct_tool_list(tool_list);
    static DropdownList tools_button(
        tool_list, Anchor(Vec2d(0.7, 0.0), Vec2d(1.4, 0.5),
                          Vec2d(-ANCHOR_DEFINITION_SIZE.x / 2.0, 0.0),
                          Vec2d(-ANCHOR_DEFINITION_SIZE.x / 2.0, 0.0)));

    tools_button.setText("Tools");
    top_menu.addChild(tools_button);

    static Panel filter_list(Anchor(Vec2d(0.8, -0.25), Vec2d(3.0, 0.0),
                                    Vec2d(0.0, 0.0), Vec2d(0.0, 0.0)));
    construct_filter_list(filter_list, canvas_view);
    static DropdownList filters_button(
        filter_list, Anchor(Vec2d(0.7 + 1.4, 0.0), Vec2d(1.4, 0.5),
                            Vec2d(-ANCHOR_DEFINITION_SIZE.x / 2.0, 0.0),
                            Vec2d(-ANCHOR_DEFINITION_SIZE.x / 2.0, 0.0)));

    filters_button.setText("Filters");
    top_menu.addChild(filters_button);

    //* REGISTRATION
    root.addChild(canvas_frame);
    root.addChild(top_menu);
    root.addChild(side_panel);
}
