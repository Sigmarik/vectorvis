#include "canvas_view.h"

#include "extern/filter_palette.h"
#include "extern/tool_palette.h"
#include "graphics/sf_cheatsheet.hpp"

CanvasView::CanvasView(const plug::LayoutBox& box, Canvas& canvas)
    : Widget(box), canvas_(canvas) {}

void CanvasView::draw(plug::TransformStack& stack, plug::RenderTarget& target) {
    static plug::VertexArray vertices(plug::PrimitiveType::Quads, 4);

    vertices[0].position = getCorner(Corner::TopLeft, stack);
    vertices[1].position = getCorner(Corner::TopRight, stack);
    vertices[2].position = getCorner(Corner::BottomRight, stack);
    vertices[3].position = getCorner(Corner::BottomLeft, stack);

    vertices[0].tex_coords = Vec2d(0.0, 0.0);
    vertices[1].tex_coords = Vec2d(1.0, 0.0);
    vertices[2].tex_coords = Vec2d(1.0, 1.0);
    vertices[3].tex_coords = Vec2d(0.0, 1.0);

    target.draw(vertices, canvas_.getTexture());
}

void CanvasView::onMouseMove(const plug::MouseMoveEvent& event,
                             plug::EHC& context) {
    plug::Tool& tool = ToolPalette::getTool();

    tool.onMove(getPixelPos(event.pos, context.stack));

    if (!covers(context.stack, event.pos)) context.stopped = true;
}

void CanvasView::onMousePressed(const plug::MousePressedEvent& event,
                                plug::EHC& context) {
    if (!covers(context.stack, event.pos)) return;

    if (context.overlapped) return;

    Vec2d pos = getPixelPos(event.pos, context.stack);

    plug::ControlState state =
        (plug::ControlState){.state = plug::State::Pressed};

    plug::Tool& tool = ToolPalette::getTool();

    switch (event.button_id) {
        case plug::MouseButton::Left: {
            tool.onMainButton(state, pos);
        } break;
        case plug::MouseButton::Right: {
            tool.onSecondaryButton(state, pos);
        } break;
        default:
            break;
    }

    context.stopped = true;
}

void CanvasView::onMouseReleased(const plug::MouseReleasedEvent& event,
                                 plug::EHC& context) {
    Vec2d pos = getPixelPos(event.pos, context.stack);

    plug::ControlState state =
        (plug::ControlState){.state = plug::State::Released};

    plug::Tool& tool = ToolPalette::getTool();

    switch (event.button_id) {
        case plug::MouseButton::Left: {
            tool.onMainButton(state, pos);
        } break;
        case plug::MouseButton::Right: {
            tool.onSecondaryButton(state, pos);
        } break;
        default:
            break;
    }
}

void CanvasView::onKeyboardPressed(const plug::KeyboardPressedEvent& event,
                                   plug::EHC& context) {
    plug::Filter& filter = FilterPalette::getFilter();

    plug::Tool& tool = ToolPalette::getTool();

    plug::ControlState state =
        (plug::ControlState){.state = plug::State::Pressed};

    switch (event.key_id) {
        case plug::KeyCode::Escape: {
            tool.onCancel();
            context.stopped = true;
        } break;
        case plug::KeyCode::Enter: {
            tool.onConfirm();
            context.stopped = true;
        } break;
        case plug::KeyCode::LShift: {
            tool.onModifier1(state);
            context.stopped = true;
        } break;
        case plug::KeyCode::LControl: {
            tool.onModifier2(state);
            context.stopped = true;
        } break;
        case plug::KeyCode::LAlt: {
            tool.onModifier3(state);
            context.stopped = true;
        } break;
        default:
            break;
    }

    if (event.key_id > plug::KeyCode::Num0 &&
        event.key_id <= plug::KeyCode::Num9) {
        ToolPalette::selectTool((unsigned)event.key_id -
                                (unsigned)plug::KeyCode::Num1);
    }
}

void CanvasView::onKeyboardReleased(const plug::KeyboardReleasedEvent& event,
                                    plug::EHC& context) {}

Vec2d CanvasView::getPixelPos(const Vec2d& screen_pos,
                              const plug::TransformStack& stack) {
    Vec2d pos = screen_pos;

    pos = stack.restore(pos) / getLayoutBox().getSize();
    pos += Vec2d(0.5, 0.5);
    pos *= canvas_.getSize();

    return pos;
}
