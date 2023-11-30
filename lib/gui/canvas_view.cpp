#include "canvas_view.h"

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
                             plug::EHC& context) {}

void CanvasView::onMousePressed(const plug::MousePressedEvent& event,
                                plug::EHC& context) {}

void CanvasView::onMouseReleased(const plug::MouseReleasedEvent& event,
                                 plug::EHC& context) {}

void CanvasView::onKeyboardPressed(const plug::KeyboardPressedEvent& event,
                                   plug::EHC& context) {}
