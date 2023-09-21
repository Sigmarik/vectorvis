#include "gui.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Mouse.hpp>

#include "sf_cheatsheet.hpp"

Panel::Panel(const Vec2d& center, const Vec2d& size)
    : children_(), interactive_children_() {
    center_ = center;
    size_ = size;
}

void Panel::render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                   const AssetShelf& assets) {
    static sf::VertexArray shape(sf::PrimitiveType::Quads, 4);

    Vec3d center = extrude(center_);
    Vec3d size = extrude(size_);

    // clang-format off
    shape[0].position = to_Vector2f(stack.top() * (center + size * Vec3d(-0.5, 0.5, 0.0)));
    shape[1].position = to_Vector2f(stack.top() * (center + size * Vec3d(0.5, 0.5, 0.0)));
    shape[2].position = to_Vector2f(stack.top() * (center + size * Vec3d(0.5, -0.5, 0.0)));
    shape[3].position = to_Vector2f(stack.top() * (center + size * Vec3d(-0.5, -0.5, 0.0)));
    // clang-format on

    shape[0].texCoords = sf::Vector2f(0.0, 0.0);
    shape[1].texCoords = sf::Vector2f(1.0, 0.0);
    shape[2].texCoords = sf::Vector2f(1.0, 1.0);
    shape[3].texCoords = sf::Vector2f(0.0, 1.0);

    // shape.setFillColor(sf::Color(45, 46, 61));

    // shape.setOutlineThickness(-0.001f);
    // shape.setOutlineColor(sf::Color::Black);

    fill_shader_parameters(stack, target, assets);

    target.draw(shape, &assets.panel_design.shader);

    stack.push(get_matrix());
    for (size_t child_id = 0; child_id < children_.size(); ++child_id) {
        children_[child_id]->render(stack, target, assets);
    }
    for (size_t child_id = 0; child_id < interactive_children_.size();
         ++child_id) {
        interactive_children_[child_id]->render(stack, target, assets);
    }
    stack.pop();
}

void Panel::on_event(MatrixStack<Mat33d>& stack, Interaction interaction) {
    Interactive::on_event(stack, interaction);
    if (movable_ && interaction.event->type == sf::Event::MouseMoved) {
        double cursor_x = (double)sf::Mouse::getPosition().x;
        double cursor_y = (double)sf::Mouse::getPosition().y;
        double dx = (cursor_x - last_cursor_position_.get_x()) /
                    interaction.window->getSize().x;
        double dy = (cursor_y - last_cursor_position_.get_y()) /
                    interaction.window->getSize().y;

        last_cursor_position_.set_x(cursor_x);
        last_cursor_position_.set_y(cursor_y);

        Vec3d shift = stack.top().inverse() * Vec3d(dx, dy, 0);
        center_ += Vec2d(shift.get_x(), shift.get_y());
    }

    stack.push(get_matrix());
    for (size_t child_id = interactive_children_.size() - 1;
         child_id != (size_t)-1; --child_id) {
        Vec2d mouse_pos = Vec2d((double)interaction.event->mouseButton.x /
                                    interaction.window->getSize().x,
                                (double)interaction.event->mouseButton.y /
                                    interaction.window->getSize().y);

        interactive_children_[child_id]->on_event(stack, interaction);

        interaction.obstructed =
            interaction.obstructed ||
            interactive_children_[child_id]->is_under(stack, mouse_pos);
    }
    stack.pop();
}

void Panel::add_child(Renderable& child) { children_.push(&child); }

void Panel::add_interactive_child(Interactive& child) {
    interactive_children_.push(&child);
}

bool Panel::is_movable() const { return movable_; }

void Panel::set_movable(bool movable) {
    movable_ = movable;
    last_cursor_position_.set_x((double)sf::Mouse::getPosition().x);
    last_cursor_position_.set_y((double)sf::Mouse::getPosition().y);
}

Mat33d Panel::get_matrix() {
    // clang-format off
    return Mat33d(1.0, 0.0, get_center().get_x(),
                  0.0, 1.0, get_center().get_y(),
                  0.0, 0.0, 1.0);
    // clang-format on
}

void Panel::fill_shader_parameters(MatrixStack<Mat33d>& stack,
                                   sf::RenderTarget& target,
                                   const AssetShelf& assets) {
    //! WARNING: Intentional const qualifier violation!
    AssetShelf& shelf = *(AssetShelf*)(&assets);
    sf::Shader& shader = shelf.panel_design.shader;
    shader.setParameter("time", (float)WorldTimer::get() / 1000000.0f);
    shader.setParameter("center", to_Vector2f(center_));

    Vec3d center = extrude(center_);
    Vec3d size = extrude(size_);

    Vec3d window_size = Vec3d(target.getSize().x, target.getSize().y, 1.0);

    Vec3d tl =
        stack.top() * (center + size * Vec3d(-0.5, 0.5, 0.0)) * window_size;
    Vec3d tr =
        stack.top() * (center + size * Vec3d(0.5, 0.5, 0.0)) * window_size;
    Vec3d br =
        stack.top() * (center + size * Vec3d(0.5, -0.5, 0.0)) * window_size;

    shader.setParameter("center_px",
                        to_Vector2f((stack.top() * center) * window_size));
    shader.setParameter("size", to_Vector2f(size_));

    shader.setParameter("size_px", sf::Vector2f((float)(tl - tr).length(),
                                                (float)(tr - br).length()));
    shader.setParameter("mouse", to_Vector2f(mouse_position_));
    shader.setParameter("window_size", to_Vector2f(window_size));
}

Button::Button(const Vec2d& center, const Vec2d& size, const char* text,
               const Vec3d& color)
    : text_(text), color_(color) {
    center_ = center;
    size_ = size;
}

void Button::render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                    const AssetShelf& assets) {
    static sf::VertexArray shape(sf::PrimitiveType::Quads, 4);
    static sf::Text render_text("[DEBUG TEXT]", sf::Font(), 13);

    double text_scale_x = (stack.top() * Vec3d(1.0, 0.0, 0.0)).length() / 50.0;
    double text_scale_y = (stack.top() * Vec3d(0.0, 1.0, 0.0)).length() / 50.0;

    render_text.setScale(
        sf::Vector2f((float)text_scale_x, (float)text_scale_y));

    render_text.setFont(assets.font);

    Vec3d center = extrude(center_);
    Vec3d size = extrude(size_);

    // clang-format off
    shape[0].position = to_Vector2f(stack.top() * (center + size * Vec3d(-0.5, 0.5, 0.0)));
    shape[1].position = to_Vector2f(stack.top() * (center + size * Vec3d(0.5, 0.5, 0.0)));
    shape[2].position = to_Vector2f(stack.top() * (center + size * Vec3d(0.5, -0.5, 0.0)));
    shape[3].position = to_Vector2f(stack.top() * (center + size * Vec3d(-0.5, -0.5, 0.0)));
    // clang-format on

    shape[0].texCoords = sf::Vector2f(0.0, 0.0);
    shape[1].texCoords = sf::Vector2f(1.0, 0.0);
    shape[2].texCoords = sf::Vector2f(1.0, 1.0);
    shape[3].texCoords = sf::Vector2f(0.0, 1.0);

    // shape.setFillColor(to_color(color_ * color_amplifier_));

    // shape.setOutlineThickness(-0.001f);
    // shape.setOutlineColor(is_pushed_ ? sf::Color::White : sf::Color::Black);

    fill_shader_parameters(stack, target, assets);

    target.draw(shape, &assets.button_design.shader);

    render_text.setString(text_);
    render_text.setPosition(to_Vector2f(stack.top() * center));
    render_text.setOrigin(
        sf::Vector2f(render_text.getLocalBounds().width / 2.0f,
                     render_text.getLocalBounds().height / 2.0f));

    target.draw(render_text);
}

static const double AMPL_DEFAULT = 1.0;
static const double AMPL_HOVER = 0.9;
static const double AMPL_PUSHED = 0.8;

void Button::on_event(MatrixStack<Mat33d>& stack, Interaction interaction) {
    Interactive::on_event(stack, interaction);
    Vec2d mouse_pos((double)sf::Mouse::getPosition(*interaction.window).x /
                        interaction.window->getSize().x,
                    (double)sf::Mouse::getPosition(*interaction.window).y /
                        interaction.window->getSize().y);
    bool has_mouse_over = is_under(stack, mouse_pos) && !interaction.obstructed;

    if (is_hovered_ != has_mouse_over) {
        is_hovered_ = has_mouse_over;
        hover_timer_ = 0;
        update_timers();
    }

    if (!is_pushed_) {
        // color_amplifier_ = has_mouse_over ? AMPL_HOVER : AMPL_DEFAULT;
    }

    if (interaction.event->mouseButton.button != sf::Mouse::Left) return;

    if (interaction.event->type == sf::Event::MouseButtonReleased &&
        is_pushed_) {
        is_pushed_ = false;
        // color_amplifier_ = has_mouse_over ? AMPL_HOVER : AMPL_DEFAULT;
        on_release(interaction);
        push_timer_ = 0;
        update_timers();
    }

    if (interaction.event->type == sf::Event::MouseButtonPressed &&
        has_mouse_over && !is_pushed_) {
        is_pushed_ = true;
        // color_amplifier_ = AMPL_PUSHED;
        on_push(interaction);
        push_timer_ = 0;
        update_timers();
    }
}

void Button::on_push(Interaction interaction) {}

void Button::on_release(Interaction interaction) {}

void Button::set_text(const char* text) { text_ = text; }

void Button::set_color(const Vec3d& color) { color_ = color; }

bool Button::is_pushed() { return is_pushed_; }

void Button::update_timers() {
    hover_timer_ += WorldTimer::get() - last_upd_time_;
    push_timer_ += WorldTimer::get() - last_upd_time_;
    last_upd_time_ = WorldTimer::get();
}

void Button::fill_shader_parameters(MatrixStack<Mat33d>& stack,
                                    sf::RenderTarget& target,
                                    const AssetShelf& assets) {
    //! WARNING: Intentional const qualifier validation!
    AssetShelf& shelf = *(AssetShelf*)(&assets);
    sf::Shader& shader = shelf.button_design.shader;
    shader.setParameter("time", (float)WorldTimer::get() / 1000000.0f);

    update_timers();

    shader.setParameter("hover_time", (float)(hover_timer_ / 1000000.0 *
                                              (is_hovered_ ? 1.0 : -1.0)));
    shader.setParameter("push_time", (float)(push_timer_ / 1000000.0 *
                                             (is_pushed_ ? 1.0 : -1.0)));

    shader.setParameter("center", to_Vector2f(center_));

    Vec3d center = extrude(center_);
    Vec3d size = extrude(size_);

    Vec3d window_size = Vec3d(target.getSize().x, target.getSize().y, 1.0);

    Vec3d tl =
        stack.top() * (center + size * Vec3d(-0.5, 0.5, 0.0)) * window_size;
    Vec3d tr =
        stack.top() * (center + size * Vec3d(0.5, 0.5, 0.0)) * window_size;
    Vec3d br =
        stack.top() * (center + size * Vec3d(0.5, -0.5, 0.0)) * window_size;

    shader.setParameter("center_px",
                        to_Vector2f((stack.top() * center) * window_size));
    shader.setParameter("size", to_Vector2f(size_));

    shader.setParameter("size_px", sf::Vector2f((float)(tl - tr).length(),
                                                (float)(tr - br).length()));
    shader.setParameter("mouse", to_Vector2f(mouse_position_));
    shader.setParameter("window_size", to_Vector2f(window_size));
}

DragButton::DragButton(Panel& panel)
    : Button(panel.get_size() * Vec2d(0.0, 0.5),
             Vec2d(panel.get_size().get_x(), 0.2), ""),
      panel_(panel) {}

void DragButton::on_push(Interaction interaction) { panel_.set_movable(true); }

void DragButton::on_release(Interaction interaction) {
    panel_.set_movable(false);
}