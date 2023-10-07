/**
 * @file image_scrollbar.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Scrollbar bound to image
 * @version 0.1
 * @date 2023-10-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef IMAGE_SCROLLBAR_H
#define IAMGE_SCROLLBAR_H

#include "scrollbar.h"
#include "src/graphics/renderable/image_view.h"

struct ImageScrollbar : Scrollbar {
    ImageScrollbar(Vec2d center, double length, ImageView& view,
                   ScrollbarType type = SCRLLBR_VERTICAL);

    void on_update(double percentage) override;

   private:
    ImageView& view_;
};

#endif
