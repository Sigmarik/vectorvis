#include "scrollbar.h"

#include <math.h>

#include "anchor.h"
#include "world_timer.h"

ScrollbarBtn::ScrollbarBtn(bool additive, Scrollbar& owner,
                           const plug::LayoutBox& box)
    : Button(box), owner_(owner), additive_(additive) {}

static const double BTN_PUSH_DELTA = 0.1;

void ScrollbarBtn::onPush() {
    double delta = BTN_PUSH_DELTA;

    if (!additive_) delta *= -1;

    owner_.setValue(owner_.getValue() + delta);

    push_time_ = WorldTimer::get();
}

static const double BTN_HOLD_DELTA = 0.6;

void ScrollbarBtn::onTick(const plug::TickEvent& event, plug::EHC& context) {
    if (!isPushed()) return;
    if (WorldTimer::get() - push_time_ < 1000000) return;

    double delta = BTN_HOLD_DELTA * event.delta_time;

    if (!additive_) delta *= -1;

    owner_.setValue(owner_.getValue() + delta);
}

void Scrollbit::onMouseMove(const plug::MouseMoveEvent& event,
                            plug::EHC& context) {
    if (isPushed()) {
        Vec2d delta = context.stack.top().restore(event.pos) -
                      context.stack.top().restore(old_mouse_pos_);

        getLayoutBox().setPosition(getLayoutBox().getPosition() + delta);
    }

    Button::onMouseMove(event, context);

    old_mouse_pos_ = event.pos;
}

static const double BAR_WIDTH = 0.5;

Scrollbar::Scrollbar(ScrollbarType type, const Vec2d& center, double length,
                     const Vec2d& bind)
    : Panel(Anchor(center,
                   type == ScrollbarType::Horizontal ? Vec2d(length, BAR_WIDTH)
                                                     : Vec2d(BAR_WIDTH, length),
                   bind, bind)),
      type_(type),
      up_btn_(true, *this,
              Anchor(Vec2d(-1.0, -1.0) * BAR_WIDTH / 2.0,
                     Vec2d(BAR_WIDTH, BAR_WIDTH), ANCHOR_DEFINITION_SIZE / 2.0,
                     ANCHOR_DEFINITION_SIZE / 2.0)),
      down_btn_(
          false, *this,
          Anchor(Vec2d(1.0, 1.0) * BAR_WIDTH / 2.0, Vec2d(BAR_WIDTH, BAR_WIDTH),
                 ANCHOR_DEFINITION_SIZE / -2.0, ANCHOR_DEFINITION_SIZE / -2.0)),
      scrollbit_(Anchor(Vec2d(0.0, 0.0), Vec2d(BAR_WIDTH, BAR_WIDTH),
                        Vec2d(0.0, 0.0), Vec2d(0.0, 0.0))) {
    up_btn_.setText(type == ScrollbarType::Horizontal ? ">" : "^");
    down_btn_.setText(type == ScrollbarType::Horizontal ? "<" : "v");
    scrollbit_.setText("");

    addChild(up_btn_);
    addChild(down_btn_);
    addChild(scrollbit_);
}

void Scrollbar::onEvent(const plug::Event& event, plug::EHC& context) {
    Panel::onEvent(event, context);

    double length = type_ == ScrollbarType::Horizontal
                        ? getLayoutBox().getSize().x
                        : getLayoutBox().getSize().y;
    length -= BAR_WIDTH * 3.0;

    Vec2d pos = scrollbit_.getLayoutBox().getPosition();

    double linear_pos = type_ == ScrollbarType::Horizontal ? pos.x : pos.y;

    setValue(linear_pos / length + 0.5);
}

void Scrollbar::setValue(double value) {
    double length = type_ == ScrollbarType::Horizontal
                        ? getLayoutBox().getSize().x
                        : getLayoutBox().getSize().y;
    length -= BAR_WIDTH * 3.0;

    if (value < 0.0) value = 0.0;
    if (value > 1.0) value = 1.0;

    double position = length * (value - 0.5);

    scrollbit_.getLayoutBox().setPosition(type_ == ScrollbarType::Horizontal
                                              ? Vec2d(position, 0.0)
                                              : Vec2d(0.0, position));

    double old_value = value_;

    value_ = value;

    if (abs(old_value - value) >= 1e-5) onUpdate(value);
}
