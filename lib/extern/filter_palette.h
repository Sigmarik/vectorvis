/**
 * @file filter_palette.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Filter palette class
 * @version 0.1
 * @date 2023-12-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __EXTERN_FILTER_PALETTE_H
#define __EXTERN_FILTER_PALETTE_H

#include "Plug/Filter.h"
#include "data_structures/stack.h"

struct FilterPalette {
    static void addFilter(plug::Filter& filter);
    static void setActive(unsigned id);
    static size_t activeId() { return id_; }

    static plug::Filter& getFilter();
    static plug::Filter& getFilter(size_t id);

    static size_t size() { return filters_.size(); }

   private:
    FilterPalette() = default;

    static FilterPalette instance_;

    static Stack<plug::Filter*> filters_;

    static unsigned id_;
};

#endif
