#include "tools.h"

void EraserTool::use(sf::Image& image, const Vec2d& pos) {
    for (int sft_x = -(int)radius_; sft_x < (int)radius_; ++sft_x) {
        for (int sft_y = -(int)radius_; sft_y < (int)radius_; ++sft_y) {
            Vec2d pixel_pos = pos + Vec2d((double)sft_x, (double)sft_y);
            if (sft_x * sft_x + sft_y * sft_y > (int)(radius_ * radius_))
                continue;
            if (pixel_pos.get_x() < 0.0 || pixel_pos.get_y() < 0.0 ||
                pixel_pos.get_x() >= image.getSize().x + 1 ||
                pixel_pos.get_y() >= image.getSize().y + 1)
                continue;
            image.setPixel((unsigned)pixel_pos.get_x(),
                           (unsigned)pixel_pos.get_y(), sf::Color(0, 0, 0));
        }
    }
}
