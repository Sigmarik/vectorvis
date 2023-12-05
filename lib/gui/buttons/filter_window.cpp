#include "filter_window.h"

#include "gui/anchor.h"

FilterWindow::FilterWindow(CanvasView& canvas_view, plug::Filter& filter)
    : Panel(Anchor(canvas_view.getCanvas().getSize() / 2.0,
                   (filter.getWidget() != nullptr
                        ? filter.getWidget()->getLayoutBox().getSize()
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
      filter_view_(filter) {
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

FilterSettingsHolder::FilterSettingsHolder(plug::Filter& filter)
    : Panel(Anchor(Vec2d(0.0, 50.0),
                   (filter.getWidget() != nullptr
                        ? filter.getWidget()->getLayoutBox().getSize()
                        : Vec2d(500.0, 500.0)),
                   Vec2d(0.0, 0.0), Vec2d(0.0, 0.0))),
      filter_(filter) {
    if (filter.getWidget() != nullptr) addChild(*filter.getWidget());
}

void FilterSettingsHolder::draw(plug::TransformStack& stack,
                                plug::RenderTarget& target) {
    stack.enter(getLocalCoords());
    filter_.getWidget()->draw(stack, target);
    stack.leave();

    Panel::draw(stack, target);
}
