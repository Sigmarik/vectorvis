#include "gui.h"

#include <GL/glew.h>

#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/OpenGL.hpp>

#include "Impl/Graphics/RenderTarget.h"
#include "anchor.h"
#include "environment.h"
#include "world_timer.h"

static sf::Vector2f to_vector2f(const Vec2d& vector) {
    return sf::Vector2f((float)vector.x, (float)vector.y);
}

void Panel::draw(plug::TransformStack& stack, plug::RenderTarget& target) {
    static plug::VertexArray vertices(plug::PrimitiveType::Quads, 9 * 4);

    DesignDescriptor design = AssetShelf::getDesign(design_);

    constructMesh(vertices, stack);

    target.draw(vertices, AssetShelf::getDesign(design_).image);

    stack.enter(getLocalCoords());

    for (size_t id = 0; id < children_.size(); ++id) {
        children_[id]->draw(stack, target);
    }

    stack.leave();
}

void Panel::onEvent(const plug::Event& event, plug::EHC& context) {
    context.stack.enter(getLocalCoords());

    for (size_t id = children_.size(); id != 0; --id) {
        children_[id - 1]->onEvent(event, context);
        if (context.stopped) break;
    }

    context.stack.leave();

    Widget::onEvent(event, context);
}

void Panel::onMouseMove(const plug::MouseMoveEvent& event, plug::EHC& context) {
    if (follows_mouse_) {
        Vec2d delta = context.stack.top().restore(event.pos) -
                      context.stack.top().restore(old_mouse_pos_);

        getLayoutBox().setPosition(getLayoutBox().getPosition() + delta);
    }
    old_mouse_pos_ = event.pos;

    Widget::onMouseMove(event, context);
}

void Panel::addChild(plug::Widget& widget) { children_.push(&widget); }

void Panel::onParentUpdate(const plug::LayoutBox& parent_box) {
    Widget::onParentUpdate(parent_box);

    for (size_t id = 0; id < children_.size(); ++id) {
        children_[id]->onParentUpdate(parent_box);
    }
}

static const double DPU = 50.0;

void Panel::constructMesh(plug::VertexArray& array,
                          const plug::TransformStack& stack) {
    plug::Vertex verts[4][4];

    DesignDescriptor design = AssetShelf::getDesign(design_);

    double abs_margin = design.margin;

    double shift = 1.5 * abs_margin / DPU;

    Vec2d uv_margin = Vec2d((double)abs_margin, (double)abs_margin) /
                      Vec2d(design.image.width, design.image.height);

    // clang-format off
    verts[0][3].position = getAbsCorner(Corner::TopLeft);
    verts[3][3].position = getAbsCorner(Corner::TopRight);
    verts[3][0].position = getAbsCorner(Corner::BottomRight);
    verts[0][0].position = getAbsCorner(Corner::BottomLeft);

    verts[0][3].tex_coords = Vec2d(0.0, 1.0);
    verts[3][3].tex_coords = Vec2d(1.0, 1.0);
    verts[3][0].tex_coords = Vec2d(1.0, 0.0);
    verts[0][0].tex_coords = Vec2d(0.0, 0.0);

    verts[0][1].position = verts[0][0].position + Vec2d(0.0, shift);
    verts[1][0].position = verts[0][0].position + Vec2d(shift, 0.0);
    verts[1][1].position = verts[0][0].position + Vec2d(shift, shift);
    verts[0][1].tex_coords = verts[0][0].tex_coords + Vec2d(0.0, uv_margin.y);
    verts[1][0].tex_coords = verts[0][0].tex_coords + Vec2d(uv_margin.x, 0.0);
    verts[1][1].tex_coords = verts[0][0].tex_coords + Vec2d(uv_margin.x, uv_margin.y);

    verts[0][2].position = verts[0][3].position + Vec2d(0.0, -shift);
    verts[1][3].position = verts[0][3].position + Vec2d(shift, 0.0);
    verts[1][2].position = verts[0][3].position + Vec2d(shift, -shift);
    verts[0][2].tex_coords = verts[0][3].tex_coords + Vec2d(0.0, -uv_margin.y);
    verts[1][3].tex_coords = verts[0][3].tex_coords + Vec2d(uv_margin.x, 0.0);
    verts[1][2].tex_coords = verts[0][3].tex_coords + Vec2d(uv_margin.x, -uv_margin.y);

    verts[3][2].position = verts[3][3].position + Vec2d(0.0, -shift);
    verts[2][3].position = verts[3][3].position + Vec2d(-shift, 0.0);
    verts[2][2].position = verts[3][3].position + Vec2d(-shift, -shift);
    verts[3][2].tex_coords = verts[3][3].tex_coords + Vec2d(0.0, -uv_margin.y);
    verts[2][3].tex_coords = verts[3][3].tex_coords + Vec2d(-uv_margin.x, 0.0);
    verts[2][2].tex_coords = verts[3][3].tex_coords + Vec2d(-uv_margin.x, -uv_margin.y);

    verts[3][1].position = verts[3][0].position + Vec2d(0.0, shift);
    verts[2][0].position = verts[3][0].position + Vec2d(-shift, 0.0);
    verts[2][1].position = verts[3][0].position + Vec2d(-shift, shift);
    verts[3][1].tex_coords = verts[3][0].tex_coords + Vec2d(0.0, uv_margin.y);
    verts[2][0].tex_coords = verts[3][0].tex_coords + Vec2d(-uv_margin.x, 0.0);
    verts[2][1].tex_coords = verts[3][0].tex_coords + Vec2d(-uv_margin.x, uv_margin.y);
    // clang-format on

    for (unsigned id_x = 0; id_x < 4; ++id_x) {
        for (unsigned id_y = 0; id_y < 4; ++id_y) {
            verts[id_x][id_y].position =
                stack.apply(verts[id_x][id_y].position);
        }
    }

    unsigned id = 0;

    for (unsigned quad_x = 0; quad_x < 3; ++quad_x) {
        for (unsigned quad_y = 0; quad_y < 3; ++quad_y) {
            array[id++] = verts[quad_x][quad_y];
            array[id++] = verts[quad_x + 1][quad_y];
            array[id++] = verts[quad_x + 1][quad_y + 1];
            array[id++] = verts[quad_x][quad_y + 1];
        }
    }
}

static plug::Transform inverse(const plug::Transform& transform) {
    Vec2d inverse_scale =
        Vec2d(1.0 / transform.getScale().x, 1.0 / transform.getScale().y);
    Vec2d inverse_offset = transform.getOffset() * -1.0 / transform.getScale();

    return plug::Transform(inverse_offset, inverse_scale);
}

static sf::Glsl::Mat3 get_matrix(const plug::Transform& transform) {
    // clang-format off
    sf::Glsl::Mat3 sf_matrix({(float)transform.getScale().x, 0.0f,
                              (float)transform.getOffset().x,
                              0.0f, (float)transform.getScale().y,
                              (float)transform.getOffset().y,
                              0.0f, 0.0f, 1.0f});
    // clang-format on

    return sf_matrix;
}

void Button::draw(plug::TransformStack& stack, plug::RenderTarget& target) {
    // TODO: Implement

    Widget::draw(stack, target);
}

void Button::onTick(const plug::TickEvent& event, plug::EHC& context) {
    unsigned long long delta_time =
        (unsigned long long)(event.delta_time * 1000000.0);

    if (is_pushed_) {
        push_timer_ += delta_time;
    } else {
        push_timer_ -= delta_time;
    }

    if (is_hovered_) {
        hover_timer_ += delta_time;
    } else {
        hover_timer_ -= delta_time;
    }
}

void Button::onMouseMove(const plug::MouseMoveEvent& event,
                         plug::EHC& context) {
    bool hover = !context.overlapped && covers(context.stack, event.pos);

    if (hover && !is_hovered_) {
        is_hovered_ = true;
        hover_timer_ = 0.0;

        onHover();
    }

    if (!hover && is_hovered_) {
        is_hovered_ = false;
        hover_timer_ = 0.0;

        onUnhover();
    }

    Widget::onMouseMove(event, context);
}

void Button::onMousePressed(const plug::MousePressedEvent& event,
                            plug::EHC& context) {
    bool cover = !context.overlapped && covers(context.stack, event.pos);

    if (cover && !is_pushed_) {
        is_pushed_ = true;
        push_timer_ = 0.0;

        onPush();
    }

    Widget::onMousePressed(event, context);
}

void Button::onMouseReleased(const plug::MouseReleasedEvent& event,
                             plug::EHC& context) {
    if (is_pushed_) {
        is_pushed_ = false;
        push_timer_ = 0.0;

        onRelease();
    }

    Widget::onMouseReleased(event, context);
}

plug::Transform Panel::getLocalCoords() const {
    return plug::Transform(getLayoutBox().getPosition());
}

static const Anchor DRAG_BTN_ANCHOR(Vec2d(0.0, 0.0),
                                    ANCHOR_DEFINITION_SIZE* Vec2d(1.0, 0.0) +
                                        Vec2d(0.0, 0.2),
                                    ANCHOR_DEFINITION_SIZE* Vec2d(-0.5, 0.5),
                                    ANCHOR_DEFINITION_SIZE* Vec2d(0.5, 0.5));

DragButton::DragButton(Panel& panel) : Button(DRAG_BTN_ANCHOR), panel_(panel) {
    panel_.addChild(*this);
}

void DragButton::onPush() { panel_.setFollowsMouse(true); }

void DragButton::onRelease() { panel_.setFollowsMouse(false); }
