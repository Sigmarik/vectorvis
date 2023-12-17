#include "dropdown_list.h"

#include "anchor.h"

void DropdownList::draw(plug::TransformStack& stack,
                        plug::RenderTarget& target) {
    if (opened_) {
        stack.enter(getLocalCoords());
        child_.draw(stack, target);
        stack.leave();
    }

    Button::draw(stack, target);
}

void DropdownList::onPush() {
    if (opened_) {
        onClose();
    } else {
        onOpen();
    }

    opened_ = !opened_;
}

void DropdownList::onEvent(const plug::Event& event, plug::EHC& context) {
    if (opened_) {
        context.stack.enter(getLocalCoords());
        child_.onEvent(event, context);
        context.stack.leave();
    }

    if (context.stopped) return;

    Button::onEvent(event, context);
}

void DropdownList::onParentUpdate(const plug::LayoutBox& parent_box) {
    Button::onParentUpdate(parent_box);

    child_.onParentUpdate(getLayoutBox());
}
