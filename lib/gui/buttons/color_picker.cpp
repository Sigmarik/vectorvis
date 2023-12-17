#include "color_picker.h"

#include "Plug/Graphics.h"
#include "gui/anchor.h"

ColorPicker::ColorPicker(ColorPalette& palette, PickerType type,
                         const plug::LayoutBox& box)
    : DropdownList(editor_, box),
      subject_(palette),
      type_(type),
      editor_(Anchor(Vec2d(-2.75, -1.5), Vec2d(5.5, 2.5),
                     ANCHOR_DEFINITION_SIZE * Vec2d(0.5, -0.5),
                     ANCHOR_DEFINITION_SIZE * Vec2d(0.5, -0.5))),
      red_bar_(ScrollbarType::Horizontal, Vec2d(0.0, 0.75), 5.0),
      green_bar_(ScrollbarType::Horizontal, Vec2d(0.0, 0.0), 5.0),
      blue_bar_(ScrollbarType::Horizontal, Vec2d(0.0, -0.75), 5.0) {
    setText("");

    editor_.addChild(red_bar_);
    editor_.addChild(green_bar_);
    editor_.addChild(blue_bar_);

    red_bar_.setText("R");
    green_bar_.setText("G");
    blue_bar_.setText("B");

    plug::Color color = palette.getFGColor();
    if (type == PickerType::Background) {
        color = palette.getBGColor();
    }

    red_bar_.setValue((double)color.r / 255.0);
    green_bar_.setValue((double)color.g / 255.0);
    blue_bar_.setValue((double)color.b / 255.0);
}

void ColorPicker::draw(plug::TransformStack& stack,
                       plug::RenderTarget& target) {
    DropdownList::draw(stack, target);

    static plug::VertexArray vertices(plug::PrimitiveType::Quads, 5);

    // clang-format off
    vertices[0].position = getCorner(Corner::TopLeft,     stack);
    vertices[1].position = getCorner(Corner::TopRight,    stack);
    vertices[2].position = getCorner(Corner::BottomRight, stack);
    vertices[3].position = getCorner(Corner::BottomLeft,  stack);
    // clang-format on

    vertices[4] = vertices[0];

    for (size_t id = 0; id < vertices.getSize(); ++id) {
        vertices[id].color = getColor();
    }

    target.draw(vertices);
}

plug::Color ColorPicker::getColor() const {
    return plug::Color((unsigned char)(red_bar_.getValue() * 255.0),
                       (unsigned char)(green_bar_.getValue() * 255.0),
                       (unsigned char)(blue_bar_.getValue() * 255.0));
}

void ColorPicker::onClose() {
    switch (type_) {
        case PickerType::Foreground: {
            subject_.setFGColor(getColor());
        } break;
        case PickerType::Background: {
            subject_.setBGColor(getColor());
        } break;
        default:
            break;
    }
}
