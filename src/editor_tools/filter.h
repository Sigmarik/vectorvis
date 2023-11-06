/**
 * @file filter.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Image filer class
 * @version 0.1
 * @date 2023-10-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef FILTER_H
#define FILTER_H

#include "src/gui_elements/image_view.h"

struct Filter {
    virtual ~Filter() = default;
    void apply(ImageView& view, const Mask& mask);

   protected:
    virtual sf::Color apply_at(sf::Image& image, size_t pos_x,
                               size_t pos_y) = 0;
};

typedef size_t FilterId;

static const size_t FILTER_COUNT = 2;

struct FilterPalette {
    FilterPalette();
    ~FilterPalette();

    Filter* get_active_filter() const { return filters_[active_id_]; }

    void set_active_filter(FilterId id) { active_id_ = id; }
    Filter& get_filter(FilterId id) const { return *filters_[id]; }

    FilterId get_max_filter_count() const { return FILTER_COUNT; }

   private:
    FilterPalette(const FilterPalette& palette) = default;
    FilterPalette& operator=(const FilterPalette& palette) = default;

    FilterId active_id_ = 0;
    Filter* filters_[FILTER_COUNT];
};

struct EdgeDetectFilter : Filter {
   protected:
    sf::Color apply_at(sf::Image& image, size_t pos_x, size_t pos_y) override;
};

struct BlurFilter : Filter {
   protected:
    sf::Color apply_at(sf::Image& image, size_t pos_x, size_t pos_y) override;
};

#endif
