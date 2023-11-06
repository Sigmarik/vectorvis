#include "image_scrollbar.h"

#define TYPE_SPLIT(alpha, beta) (type == SCRLLBR_VERTICAL ? (alpha) : (beta))

ImageScrollbar::ImageScrollbar(Vec2d center, double length, ImageView& view,
                               ScrollbarType type)
    : Scrollbar(center, length, type), view_(view) {}

void ImageScrollbar::on_update(double percentage) {
    if (type_ == SCRLLBR_HORIZONTAL) {
        view_.scroll_x(get_percentage());
    } else {
        view_.scroll_y(1.0 - get_percentage());
    }
}
