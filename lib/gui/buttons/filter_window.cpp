#include "filter_window.h"

#include "gui/anchor.h"

static const double UNIT_SIZE = 50.0;

FilterWindow::FilterWindow(CanvasView& canvas_view, plug::Filter& filter,
                           const Vec2d& position)
    : Panel(
          Anchor(position,
                 (filter.getWidget() != nullptr
                      ? filter.getWidget()->getLayoutBox().getSize() / UNIT_SIZE
                      : Vec2d(7.0, 7.0)) +
                     Vec2d(0.5, 1.25),
                 Vec2d(0.0, 0.0), Vec2d(0.0, 0.0))),
      canvas_(canvas_view),
      filter_(filter),
      apply_btn_(*this, Anchor(Vec2d(-0.65, 0.5), Vec2d(1.2, 0.5),
                               ANCHOR_DEFINITION_SIZE * Vec2d(0.0, -0.5),
                               ANCHOR_DEFINITION_SIZE * Vec2d(0.0, -0.5))),
      cancel_btn_(*this, Anchor(Vec2d(0.65, 0.5), Vec2d(1.2, 0.5),
                                ANCHOR_DEFINITION_SIZE * Vec2d(0.0, -0.5),
                                ANCHOR_DEFINITION_SIZE * Vec2d(0.0, -0.5))),
      filter_view_(filter, *this) {
    addChild(apply_btn_);
    addChild(cancel_btn_);
    addChild(filter_view_);

    apply_btn_.setText("ok");
    cancel_btn_.setText("cancel");
}

void FilterWindow::apply() {
    filter_.applyFilter(canvas_.getCanvas());
    setActive(false);
}

void FilterWindow::cancel() { setActive(false); }

void FilterApplyButton::onRelease() { parent_.apply(); }

void FilterCancelButton::onRelease() { parent_.cancel(); }

FilterSettingsHolder::FilterSettingsHolder(plug::Filter& filter,
                                           FilterWindow& parent)
    : Panel(
          Anchor(Vec2d(0.0, 0.5),
                 filter.getWidget() == nullptr
                     ? Vec2d(7.0, 7.0)
                     : filter.getWidget()->getLayoutBox().getSize() / UNIT_SIZE,
                 Vec2d(0.0, 0.0), Vec2d(0.0, 0.0))),
      filter_(filter),
      parent_(parent) {}

void FilterSettingsHolder::draw(plug::TransformStack& stack,
                                plug::RenderTarget& target) {
    Panel::draw(stack, target);

    plug::Widget* widget = filter_.getWidget();
    if (widget != nullptr) {
        stack.enter(getScalingCoords());
        widget->draw(stack, target);
        stack.leave();
    }
}

void FilterSettingsHolder::onEvent(const plug::Event& event,
                                   plug::EHC& context) {
    plug::Widget* widget = filter_.getWidget();
    if (widget != nullptr) {
        context.stack.enter(getScalingCoords());
        widget->onEvent(event, context);
        context.stack.leave();
        getLayoutBox().setSize(widget->getLayoutBox().getSize() / UNIT_SIZE);
    }

    Panel::onEvent(event, context);
}

plug::Transform FilterSettingsHolder::getScalingCoords() const {
    return plug::Transform(getLayoutBox().getPosition(),
                           Vec2d(1.0, 1.0) / UNIT_SIZE);
}
