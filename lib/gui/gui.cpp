#include "gui.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Mouse.hpp>

#include "sf_cheatsheet.hpp"

Panel::Panel(const Vec2d& center, const Vec2d& size)
    : Interactive(center, size), children_(), interactive_children_() {}

void Panel::render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                   const AssetShelf& assets) {
    static sf::VertexArray shape(sf::PrimitiveType::Quads, 4);

    // clang-format off
    shape[0].position = to_Vector2f(get_corner(TOP_LEFT,     stack));
    shape[1].position = to_Vector2f(get_corner(TOP_RIGHT,    stack));
    shape[2].position = to_Vector2f(get_corner(BOTTOM_RIGHT, stack));
    shape[3].position = to_Vector2f(get_corner(BOTTOM_LEFT,  stack));
    // clang-format on

    shape[0].texCoords = sf::Vector2f(0.0, 0.0);
    shape[1].texCoords = sf::Vector2f(1.0, 0.0);
    shape[2].texCoords = sf::Vector2f(1.0, 1.0);
    shape[3].texCoords = sf::Vector2f(0.0, 1.0);

    fill_shader_parameters(stack, target, assets);

    target.draw(shape, &assets.designs[design_].shader);

    stack.push(get_matrix());
    for (size_t child_id = 0; child_id < children_.size(); ++child_id) {
        children_[child_id]->render(stack, target, assets);
    }
    for (size_t child_id = 0; child_id < interactive_children_.size();
         ++child_id) {
        interactive_children_[child_id]->apply_anchor(vis_size_);
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
        set_center(get_center() + Vec2d(shift.get_x(), shift.get_y()));
    }

    Vec2d mouse_pos =
        Vec2d((double)sf::Mouse::getPosition(*interaction.window).x /
                  interaction.window->getSize().x,
              (double)sf::Mouse::getPosition(*interaction.window).y /
                  interaction.window->getSize().y);

    stack.push(get_matrix());
    for (size_t child_id = interactive_children_.size() - 1;
         child_id != (size_t)-1; --child_id) {
        Interactive& child = *interactive_children_[child_id];

        child.apply_anchor(vis_size_);

        bool overlap = child.is_under(stack, mouse_pos);

        child.on_event(stack, interaction);

        if (orderable_ && !interaction.obstructed && overlap &&
            interaction.event->type == sf::Event::MouseButtonPressed)
            focus_child(child);

        interaction.obstructed = interaction.obstructed || overlap;
    }
    stack.pop();
}

void Panel::tick() {
    for (size_t child_id = 0; child_id < interactive_children_.size();
         ++child_id) {
        interactive_children_[child_id]->tick();
    }
}

void Panel::add_child(Renderable& child) { children_.push(&child); }

void Panel::add_interactive_child(Interactive& child, Anchor anchor) {
    child.set_anchor(anchor);
    interactive_children_.push(&child);
}

void Panel::focus_child(const Interactive& child) {
    for (size_t id = 0; id + 1 < interactive_children_.size(); ++id) {
        if (interactive_children_[id] == &child) {
            Interactive* mem = interactive_children_[id];
            interactive_children_[id] = interactive_children_[id + 1];
            interactive_children_[id + 1] = mem;
        }
    }
}

bool Panel::is_movable() const { return movable_; }

void Panel::set_movable(bool movable) {
    movable_ = movable;
    last_cursor_position_.set_x((double)sf::Mouse::getPosition().x);
    last_cursor_position_.set_y((double)sf::Mouse::getPosition().y);
}

void Panel::fill_shader_parameters(MatrixStack<Mat33d>& stack,
                                   sf::RenderTarget& target,
                                   const AssetShelf& assets) {
    //! WARNING: Intentional const qualifier violation!
    AssetShelf& shelf = *(AssetShelf*)(&assets);
    sf::Shader& shader = shelf.designs[design_].shader;

    set_matrix_uniform(shader, "transform", stack.top());

    shader.setParameter("time", (float)WorldTimer::get() / 1000000.0f);
    shader.setParameter("center", to_Vector2f(vis_center_));

    Vec3d center = extrude(vis_center_);
    Vec3d size = extrude(vis_size_);

    Vec3d window_size = Vec3d(target.getSize().x, target.getSize().y, 1.0);

    Vec3d tl =
        stack.top() * (center + size * Vec3d(-0.5, 0.5, 0.0)) * window_size;
    Vec3d tr =
        stack.top() * (center + size * Vec3d(0.5, 0.5, 0.0)) * window_size;
    Vec3d br =
        stack.top() * (center + size * Vec3d(0.5, -0.5, 0.0)) * window_size;

    shader.setParameter("center_px",
                        to_Vector2f((stack.top() * center) * window_size));
    shader.setParameter("size", to_Vector2f(vis_size_));

    shader.setParameter("size_px", sf::Vector2f((float)(tl - tr).length(),
                                                (float)(tr - br).length()));
    shader.setParameter("mouse", to_Vector2f(mouse_position_));
    shader.setParameter("window_size", to_Vector2f(window_size));
}

Button::Button(const Vec2d& center, const Vec2d& size, const char* text)
    : Interactive(center, size), text_(text) {}

void Button::render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                    const AssetShelf& assets) {
    static sf::VertexArray shape(sf::PrimitiveType::Quads, 4);
    static sf::Text render_text("[DEBUG TEXT]", sf::Font(), 13);

    double text_scale_x = (stack.top() * Vec3d(1.0, 0.0, 0.0)).length() / 50.0;
    double text_scale_y = (stack.top() * Vec3d(0.0, 1.0, 0.0)).length() / 50.0;

    render_text.setScale(
        sf::Vector2f((float)text_scale_x, (float)text_scale_y));

    render_text.setFont(assets.font);

    Vec3d center = extrude(vis_center_);
    Vec3d size = extrude(vis_size_);

    // clang-format off
    shape[0].position = to_Vector2f(get_corner(TOP_LEFT,     stack));
    shape[1].position = to_Vector2f(get_corner(TOP_RIGHT,    stack));
    shape[2].position = to_Vector2f(get_corner(BOTTOM_RIGHT, stack));
    shape[3].position = to_Vector2f(get_corner(BOTTOM_LEFT,  stack));
    // clang-format on

    shape[0].texCoords = sf::Vector2f(0.0, 0.0);
    shape[1].texCoords = sf::Vector2f(1.0, 0.0);
    shape[2].texCoords = sf::Vector2f(1.0, 1.0);
    shape[3].texCoords = sf::Vector2f(0.0, 1.0);

    fill_shader_parameters(stack, target, assets);

    target.draw(shape, &assets.designs[design_].shader);

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

    //* Uncomment for fun!
    // if (is_under(stack, mouse_pos) && !interaction.obstructed) {
    //     set_design(DSGN_PANEL_DEBUG);
    // }

    bool has_mouse_over = is_under(stack, mouse_pos) && !interaction.obstructed;

    if (is_hovered_ != has_mouse_over) {
        is_hovered_ = has_mouse_over;
        hover_timer_ = 0;
        update_timers();
    }

    if (interaction.event->mouseButton.button != sf::Mouse::Left) return;

    if (interaction.event->type == sf::Event::MouseButtonReleased &&
        is_pushed_) {
        is_pushed_ = false;
        on_release(interaction);
        push_timer_ = 0;
        update_timers();
    }

    if (interaction.event->type == sf::Event::MouseButtonPressed &&
        has_mouse_over && !is_pushed_) {
        is_pushed_ = true;
        on_push(interaction);
        push_timer_ = 0;
        update_timers();
    }
}

bool Button::is_pushed() { return is_pushed_; }

void Button::update_timers() {
    hover_timer_ += WorldTimer::get() - last_upd_time_;
    push_timer_ += WorldTimer::get() - last_upd_time_;
    last_upd_time_ = WorldTimer::get();
}

void Button::fill_shader_parameters(MatrixStack<Mat33d>& stack,
                                    sf::RenderTarget& target,
                                    const AssetShelf& assets) {
    //! WARNING: Intentional const qualifier violation!
    AssetShelf& shelf = *(AssetShelf*)(&assets);
    sf::Shader& shader = shelf.designs[design_].shader;

    set_matrix_uniform(shader, "transform", stack.top());

    shader.setParameter("time", (float)WorldTimer::get() / 1000000.0f);

    update_timers();

    shader.setParameter("hover_time", ((float)hover_timer_ / 1000000.0f *
                                       (is_hovered_ ? 1.0f : -1.0f)));
    shader.setParameter("push_time", ((float)push_timer_ / 1000000.0f *
                                      (is_pushed_ ? 1.0f : -1.0f)));

    shader.setParameter("center", to_Vector2f(vis_center_));

    Vec3d center = extrude(vis_center_);
    Vec3d size = extrude(vis_size_);

    Vec3d window_size = Vec3d(target.getSize().x, target.getSize().y, 1.0);

    Vec3d tl =
        stack.top() * (center + size * Vec3d(-0.5, 0.5, 0.0)) * window_size;
    Vec3d tr =
        stack.top() * (center + size * Vec3d(0.5, 0.5, 0.0)) * window_size;
    Vec3d br =
        stack.top() * (center + size * Vec3d(0.5, -0.5, 0.0)) * window_size;

    shader.setParameter("center_px",
                        to_Vector2f((stack.top() * center) * window_size));
    shader.setParameter("size", to_Vector2f(vis_size_));

    shader.setParameter("size_px", sf::Vector2f((float)(tl - tr).length(),
                                                (float)(tr - br).length()));
    shader.setParameter("mouse", to_Vector2f(mouse_position_));
    shader.setParameter("window_size", to_Vector2f(window_size));
}

DragButton::DragButton(Panel& panel)
    : Button(Vec2d(0.0, -0.1), Vec2d(1.0, 0.2), ""), panel_(panel) {
    set_design(DSGN_BUTTON_DRAG);
    panel.add_interactive_child(*this,
                                Anchor(Vec2d(0.0, 0.5), Vec2d(1.0, 0.0)));
}

void DragButton::on_push(Interaction interaction) { panel_.set_movable(true); }

void DragButton::on_release(Interaction interaction) {
    panel_.set_movable(false);
}
