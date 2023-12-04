#include "filter_palette.h"

FilterPalette FilterPalette::instance_ = FilterPalette();

Stack<plug::Filter*> FilterPalette::filters_ = Stack<plug::Filter*>();

unsigned FilterPalette::id_ = 0;

void FilterPalette::addFilter(plug::Filter& filter) { filters_.push(&filter); }

void FilterPalette::setActive(unsigned id) {
    assert(id < filters_.size());

    id_ = id;
}

plug::Filter& FilterPalette::getFilter() {
    assert(filters_.size());

    return *filters_[id_];
}
