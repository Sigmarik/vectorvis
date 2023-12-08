#include "gui.h"

#include <GL/glew.h>

#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/OpenGL.hpp>

#include "Impl/Graphics/RenderTarget.h"
#include "anchor.h"
#include "environment.h"
#include "graphics/sf_cheatsheet.hpp"
#include "world_timer.h"

void Panel::draw(plug::TransformStack& stack, plug::RenderTarget& target) {
    static plug::VertexArray vertices(plug::PrimitiveType::Quads, 9 * 4);

    constructMesh(vertices, stack);

    plug::Texture& texture = AssetShelf::getDesign(getDesign()).image;

    target.draw(vertices, texture);

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
    if (!is_mp_valid_) {
        old_mouse_pos_ = event.pos;
        is_mp_valid_ = true;
    }

    if (follows_mouse_) {
        Vec2d delta = context.stack.top().restore(event.pos) -
                      context.stack.top().restore(old_mouse_pos_);

        getLayoutBox().setPosition(getLayoutBox().getPosition() + delta);
    }
    old_mouse_pos_ = event.pos;

    Widget::onMouseMove(event, context);
}

void Panel::addChild(plug::Widget& widget) {
    children_.push(&widget);
    widget.onParentUpdate(getLayoutBox());
}

void Panel::onParentUpdate(const plug::LayoutBox& parent_box) {
    Widget::onParentUpdate(parent_box);

    for (size_t id = 0; id < children_.size(); ++id) {
        children_[id]->onParentUpdate(getLayoutBox());
    }
}

static const double DPU = 50.0;

void Designable::constructMesh(plug::VertexArray& array,
                               const plug::TransformStack& stack) {
    plug::Vertex verts[4][4];

    DesignDescriptor design = AssetShelf::getDesign(design_);

    double abs_margin = design.margin;

    double shift = 1.5 * abs_margin / DPU;

    Vec2d uv_margin =
        Vec2d(abs_margin, abs_margin) /
        Vec2d((double)design.image.width, (double)design.image.height);

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
            plug::Vertex& vertex = verts[id_x][id_y];

            vertex.position = stack.apply(vertex.position);
            vertex.tex_coords =
                Vec2d(vertex.tex_coords.x, 1.0 - vertex.tex_coords.y);
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

static const unsigned TEXT_RECT_WIDTH = 512;
static const unsigned TEXT_RECT_HEIGHT = 32;

static double render_text(plug::Texture& texture, const char* text) {
    static sf::RenderTexture plot;

    plot.create(TEXT_RECT_WIDTH, TEXT_RECT_HEIGHT);
    plot.clear(sf::Color(0, 0, 0, 0));

    sf::Text renderable_text(text, AssetShelf::getFont(), 6);

    renderable_text.setStyle(sf::Text::Bold);

    renderable_text.setOrigin(0.0, 0.0);
    renderable_text.setPosition(0.0, 0.0);

    renderable_text.setColor(sf::Color::White);
    renderable_text.setCharacterSize(TEXT_RECT_HEIGHT);

    plot.draw(renderable_text);

    plot.display();

    sf::Image image = plot.getTexture().copyToImage();

    to_plug_texture(image, texture);

    return renderable_text.getGlobalBounds().width;
}

static const double TEXT_SIZE = 0.26;

void Button::draw(plug::TransformStack& stack, plug::RenderTarget& target) {
    static plug::VertexArray vertices(plug::PrimitiveType::Quads, 9 * 4);

    constructMesh(vertices, stack);

    target.draw(vertices, AssetShelf::getDesign(getDesign()).image);

    plug::Texture text_texture = plug::Texture(0, 0);

    double width = render_text(text_texture, text_);

    double rel_width = width / (double)TEXT_RECT_WIDTH;

    static plug::VertexArray text_verts(plug::PrimitiveType::Quads, 4);

    text_verts[0].tex_coords = Vec2d(0.0, 1.0);
    text_verts[1].tex_coords = Vec2d(0.0, 0.0);
    text_verts[2].tex_coords = Vec2d(rel_width, 0.0);
    text_verts[3].tex_coords = Vec2d(rel_width, 1.0);

    Vec2d text_center =
        getLayoutBox().getPosition() + Vec2d(0.0, TEXT_SIZE * 0.23);

    double box_length =
        TEXT_SIZE * rel_width * TEXT_RECT_WIDTH / TEXT_RECT_HEIGHT;

    Vec2d text_tl = text_center + Vec2d(-box_length, TEXT_SIZE) / 2.0;
    Vec2d text_tr = text_center + Vec2d(box_length, TEXT_SIZE) / 2.0;
    Vec2d text_br = text_center + Vec2d(box_length, -TEXT_SIZE) / 2.0;
    Vec2d text_bl = text_center + Vec2d(-box_length, -TEXT_SIZE) / 2.0;

    text_verts[0].position = stack.top().apply(text_bl);
    text_verts[1].position = stack.top().apply(text_tl);
    text_verts[2].position = stack.top().apply(text_tr);
    text_verts[3].position = stack.top().apply(text_br);

    target.draw(text_verts, text_texture);
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

    if (is_pushed_) {
        setDesign(DSGN_PANEL_RED_INVERSE);
    } else {
        if (is_hovered_) {
            setDesign(DSGN_PANEL_RED_HIGHLIGHT);
        } else {
            setDesign(DSGN_PANEL_RED);
        }
    }
}

void Button::onMouseMove(const plug::MouseMoveEvent& event,
                         plug::EHC& context) {
    bool hover = !context.overlapped && covers(context.stack, event.pos);

    if (hover) context.overlapped = true;

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

        context.stopped = true;

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

static const Anchor DRAG_BTN_ANCHOR(Vec2d(0.0, -0.2),
                                    ANCHOR_DEFINITION_SIZE* Vec2d(1.0, 0.0) +
                                        Vec2d(0.1, 0.4),
                                    ANCHOR_DEFINITION_SIZE* Vec2d(-0.5, 0.5),
                                    ANCHOR_DEFINITION_SIZE* Vec2d(0.5, 0.5));

DragButton::DragButton(Panel& panel) : Button(DRAG_BTN_ANCHOR), panel_(panel) {
    setText("");
    panel_.addChild(*this);
}

void DragButton::onPush() { panel_.setFollowsMouse(true); }

void DragButton::onRelease() { panel_.setFollowsMouse(false); }
