#include "tool_selector.h"

static void switch_to(ToolPalette& pallet, Tool& tool) {
    if (pallet.get_tool() != nullptr) pallet.get_tool()->cancel();
    pallet.set_tool(&tool);
}

void ToolSelector::on_event(MatrixStack<Mat33d>& stack,
                            Interaction interaction) {
    if (interaction.event->type != sf::Event::KeyPressed) return;

    static CircleTool circle;
    static LineTool line;
    static PencilTool pencil;
    static StripTool strip;
    static PickerTool picker;

    switch (interaction.event->key.code) {
        case sf::Keyboard::C: {
            switch_to(pallet_, circle);
        } break;
        case sf::Keyboard::L: {
            switch_to(pallet_, line);
        } break;
        case sf::Keyboard::B: {
            switch_to(pallet_, pencil);
        } break;
        case sf::Keyboard::P: {
            switch_to(pallet_, picker);
        } break;
        case sf::Keyboard::S: {
            switch_to(pallet_, strip);
        } break;
        default:
            break;
    }
}
