#include "scrollbar.h"

ScrollbarHandle::ScrollbarHandle(ScrollbarBar& bar)
    : Button(Vec2d(0.0, 0.0), Vec2d(SCROLLBAR_WIDTH, SCROLLBAR_HANDLE_HEIGHT),
             ""),
      bar_(bar) {
    bar_.add_child(*this);
}

void ScrollbarHandle::on_event(MatrixStack<Mat33d>& stack,
                               Interaction interaction) {
    Button::on_event(stack, interaction);

    if (is_pushed()) {
        sf::Vector2i mouse_pure = sf::Mouse::getPosition(*interaction.window);
        Vec2d mouse_pos((double)mouse_pure.x / interaction.window->getSize().x,
                        (double)mouse_pure.y / interaction.window->getSize().y);

        Vec2d delta = mouse_pos - prev_mouse_pos_;
        Vec3d proc_delta =
            stack.top().inverse() * Vec3d(delta.get_x(), delta.get_y(), 0.0);

        prev_mouse_pos_ = mouse_pos;

        center_.set_y(center_.get_y() +
                      proc_delta.get_y() / bar_.get_vis_size().get_y());

        bar_.set_percentage(bar_.get_percentage());
    }
}

void ScrollbarHandle::on_push(Interaction interaction) {
    sf::Vector2i mouse_pure = sf::Mouse::getPosition(*interaction.window);
    Vec2d mouse_pos((double)mouse_pure.x / interaction.window->getSize().x,
                    (double)mouse_pure.y / interaction.window->getSize().y);

    prev_mouse_pos_ = mouse_pos;
}

ScrollbarBar::ScrollbarBar(Scrollbar& bar)
    : Panel(Vec2d(0.0, 0.0),
            Vec2d(SCROLLBAR_WIDTH, 1.0 - SCROLLBAR_WIDTH * 2.0)),
      handle_(*this),
      bar_(bar) {
    bar_.add_interactive_child(
        *this,
        Anchor(Vec2d(0.0, 0.0), Vec2d(0.0, 1.0 - SCROLLBAR_WIDTH * 2.0)));
    set_design(DSGN_PANEL_SOLID_DARK);
}

void ScrollbarBar::on_event(MatrixStack<Mat33d>& stack,
                            Interaction interaction) {
    sf::Vector2i mouse_pure = sf::Mouse::getPosition(*interaction.window);
    Vec2d mouse_pos((double)mouse_pure.x / interaction.window->getSize().x,
                    (double)mouse_pure.y / interaction.window->getSize().y);

    if (!interaction.obstructed && is_under(stack, mouse_pos)) {
        Vec3d mouse_pos_inv = stack.top().inverse() *
                              Vec3d(mouse_pos.get_x(), mouse_pos.get_y(), 1.0);

        Vec2d rel_mouse_pos =
            Vec2d(mouse_pos_inv.get_x(), mouse_pos_inv.get_y()) /
                mouse_pos_inv.get_z() -
            center_;

        if (!handle_.is_under(stack, mouse_pos) &&
            interaction.event->type == sf::Event::MouseButtonPressed &&
            interaction.event->mouseButton.button == sf::Mouse::Button::Left) {
            handle_.set_center(
                Vec2d(0.0, rel_mouse_pos.get_y() / vis_size_.get_y()));
        }

        if (interaction.event->type == sf::Event::MouseWheelScrolled) {
            handle_.set_center(Vec2d(
                0.0, handle_.get_center().get_y() +
                         0.1 * interaction.event->mouseWheelScroll.delta));
            bar_.set_percentage(bar_.get_percentage());
        }
    }

    Panel::on_event(stack, interaction);
}

double ScrollbarBar::get_percentage() const {
    double low = -0.5 + SCROLLBAR_HANDLE_HEIGHT / 2.0;
    double high = -low;

    return (handle_.get_center().get_y() - low) / (high - low);
}

void ScrollbarBar::set_percentage(double percentage) {
    percentage = percentage > 0.0 ? (percentage < 1.0 ? percentage : 1.0) : 0.0;

    double low = -0.5 + SCROLLBAR_HANDLE_HEIGHT / 2.0;
    double high = -low;

    handle_.set_center(
        Vec2d(0.0, high * percentage + low * (1.0 - percentage)));
    bar_.on_update(percentage);
}

ScrollbarControlBtn::ScrollbarControlBtn(Scrollbar& bar,
                                         ScrollbarButtonType type)
    : Button(
          Vec2d(0.0, SCROLLBAR_WIDTH / 2.0 * (type == SCRLL_UP ? -1.0 : 1.0)),
          Vec2d(1.0, SCROLLBAR_WIDTH), ""),
      bar_(bar),
      type_(type) {
    bar_.add_interactive_child(
        *this,
        Anchor(Vec2d(0.0, type == SCRLL_UP ? 0.5 : -0.5), Vec2d(1.0, 0.0)));
}

void ScrollbarControlBtn::on_push(Interaction interaction) {
    double delta = 0.02;
    bar_.set_percentage(bar_.get_percentage() +
                        (type_ == SCRLL_UP ? delta : -delta));

    push_time_ = WorldTimer::get();
}

void ScrollbarControlBtn::tick() {
    unsigned long long time = WorldTimer::get();

    // printf("Time pushed: %llu, Is pushed: %i\n", time - push_time_,
    //        is_pushed());

    if (is_pushed() && time - push_time_ > (unsigned long long)5e5) {
        double delta = (double)(time - update_time_) * 1e-6 * 0.2;
        bar_.set_percentage(bar_.get_percentage() +
                            (type_ == SCRLL_UP ? delta : -delta));
    }

    update_time_ = time;
}

Scrollbar::Scrollbar(Vec2d center, double length, ScrollbarType type)
    : Panel(center, Vec2d(SCROLLBAR_WIDTH, length)),
      type_(type),
      up_btn_(*this, SCRLL_UP),
      down_btn_(*this, SCRLL_DOWN),
      bar_(*this) {
    if (type == SCRLLBR_HORIZONTAL) {
        set_custom_transform(
            Mat33d(0.0, 1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0));
    }
}

double Scrollbar::get_percentage() const { return bar_.get_percentage(); }

void Scrollbar::set_percentage(double percentage) {
    bar_.set_percentage(percentage);
}

void Scrollbar::apply_anchor(const Vec2d& parent_size) {
    if (type_ == SCRLLBR_HORIZONTAL)
        Panel::apply_anchor(Vec2d(parent_size.get_y(), parent_size.get_x()));
    else
        Panel::apply_anchor(parent_size);
}
