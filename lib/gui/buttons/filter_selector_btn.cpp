#include "filter_selector_btn.h"

#include "extern/filter_palette.h"

FilterSelectionButton::FilterSelectionButton(size_t id, CanvasView& view,
                                             const plug::LayoutBox& box)
    : Button(box),
      id_(id),
      canvas_(view),
      filter_view_(view, FilterPalette::getFilter(id)) {}

void FilterSelectionButton::onPush() {
    FilterPalette::setActive((unsigned)id_);

    if (FilterPalette::getFilter(id_).getWidget() == nullptr) {
        FilterPalette::getFilter(id_).applyFilter(canvas_.getCanvas());
    } else {
        filter_view_.setActive(true);
        canvas_.setFilterView(filter_view_);
    }
}

void FilterSelectionButton::draw(plug::TransformStack& stack,
                                 plug::RenderTarget& target) {
    if (FilterPalette::activeId() == id_ && !isPushed()) {
        setDesign(DSGN_PANEL_RED_HIGHLIGHT);
    }

    Button::draw(stack, target);
}
