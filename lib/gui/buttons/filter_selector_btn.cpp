#include "filter_selector_btn.h"

#include "extern/filter_palette.h"

void FilterSelectionButton::onPush() { FilterPalette::setActive(id_); }

void FilterSelectionButton::draw(plug::TransformStack& stack,
                                 plug::RenderTarget& target) {
    if (FilterPalette::activeId() == id_ && !isPushed()) {
        setDesign(DSGN_PANEL_RED_HIGHLIGHT);
    }

    Button::draw(stack, target);
}
