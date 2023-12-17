#include "filter_window.h"

#include "gui/anchor.h"

FilterWindow::FilterWindow(CanvasView& canvas_view, plug::Filter& filter)
    : Panel(Anchor(canvas_view.getCanvas().getSize() / 2.0,
                   (filter.getWidget() != nullptr
                        ? filter.getWidget()->getLayoutBox().getSize() / 50.0
                        : Vec2d(500.0, 500.0)) +
                       Vec2d(0.0, 100.0),
                   Vec2d(0.0, 0.0), Vec2d(0.0, 0.0))),
      canvas_(canvas_view),
      filter_(filter),
      apply_btn_(*this, Anchor(Vec2d(-100.0, 50.0), Vec2d(175.0, 50.0),
                               ANCHOR_DEFINITION_SIZE * Vec2d(0.0, -0.5),
                               ANCHOR_DEFINITION_SIZE * Vec2d(0.0, -0.5))),
      cancel_btn_(*this, Anchor(Vec2d(100.0, 50.0), Vec2d(175.0, 50.0),
                                ANCHOR_DEFINITION_SIZE * Vec2d(0.0, -0.5),
                                ANCHOR_DEFINITION_SIZE * Vec2d(0.0, -0.5))),
      filter_view_(filter, *this) {
    addChild(apply_btn_);
    addChild(cancel_btn_);
    addChild(filter_view_);
}

void FilterWindow::apply() {
    filter_.applyFilter(canvas_.getCanvas());
    setActive(false);
}

void FilterWindow::cancel() { setActive(false); }

void FilterApplyButton::onPush() { parent_.apply(); }

void FilterCancelButton::onPush() { parent_.cancel(); }

static const double UNIT_SIZE = 50.0;

FilterSettingsHolder::FilterSettingsHolder(plug::Filter& filter,
                                           FilterWindow& parent)
    : Panel(
          Anchor(Vec2d(0.0, 0.0),
                 filter.getWidget() == nullptr
                     ? Vec2d(3.0, 3.0)
                     : filter.getWidget()->getLayoutBox().getSize() / UNIT_SIZE,
                 Vec2d(0.0, 0.0), Vec2d(0.0, 0.0))),
      filter_(filter),
      parent_(parent) {}

void FilterSettingsHolder::draw(plug::TransformStack& stack,
                                plug::RenderTarget& target) {
    if (filter_.getWidget() != nullptr) {
        stack.enter(getLocalCoords());
        filter_.getWidget()->draw(stack, target);
        stack.leave();
    }

    Panel::draw(stack, target);
}

void FilterSettingsHolder::onEvent(const plug::Event& event,
                                   plug::EHC& context) {
    // TODO: Implement
}
