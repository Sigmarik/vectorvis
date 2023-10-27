#include "filter.h"

#include <SFML/Graphics/Image.hpp>

FilterPalette::FilterPalette() {
    filters_[0] = new BlurFilter();
    filters_[1] = new EdgeDetectFilter();
}

FilterPalette::~FilterPalette() {
    for (FilterId id = 0; id < FILTER_COUNT; ++id) {
        delete filters_[id];
    }
}

sf::Color EdgeDetectFilter::apply_at(sf::Image& image, size_t pos_x,
                                     size_t pos_y) {
    int delta_r = 0, delta_g = 0, delta_b = 0;
    int counter = 0;
    for (size_t dx = (size_t)-1; dx != 2; ++dx) {
        for (size_t dy = (size_t)-1; dy != 2; ++dy) {
            if (pos_x + dx >= image.getSize().x) continue;
            if (pos_y + dy >= image.getSize().y) continue;
            sf::Color color =
                image.getPixel((unsigned)(pos_x + dx), (unsigned)(pos_y + dy));
            delta_r -= color.r;
            delta_g -= color.g;
            delta_b -= color.b;
            ++counter;
        }
    }

    sf::Color color = image.getPixel((unsigned)pos_x, (unsigned)pos_y);

    delta_r += counter * color.r;
    delta_g += counter * color.g;
    delta_b += counter * color.b;

    delta_r = abs(delta_r);
    delta_g = abs(delta_g);
    delta_b = abs(delta_b);

    delta_r /= counter;
    delta_g /= counter;
    delta_b /= counter;

    return sf::Color((unsigned char)(delta_r < 256 ? delta_r : 255),
                     (unsigned char)(delta_g < 256 ? delta_g : 255),
                     (unsigned char)(delta_b < 256 ? delta_b : 255));
}

sf::Color BlurFilter::apply_at(sf::Image& image, size_t pos_x, size_t pos_y) {
    unsigned delta_r = 0, delta_g = 0, delta_b = 0;
    unsigned counter = 0;
    for (size_t dx = (size_t)-1; dx != 2; ++dx) {
        for (size_t dy = (size_t)-1; dy != 2; ++dy) {
            if (pos_x + dx >= image.getSize().x) continue;
            if (pos_y + dy >= image.getSize().y) continue;
            sf::Color color =
                image.getPixel((unsigned)(pos_x + dx), (unsigned)(pos_y + dy));
            delta_r += color.r;
            delta_g += color.g;
            delta_b += color.b;
            ++counter;
        }
    }

    delta_r /= counter;
    delta_g /= counter;
    delta_b /= counter;

    return sf::Color((unsigned char)(delta_r < 256 ? delta_r : 255),
                     (unsigned char)(delta_g < 256 ? delta_g : 255),
                     (unsigned char)(delta_b < 256 ? delta_b : 255));
}

void Filter::apply(ImageView& view, const Mask& mask) {
    sf::Image& image = view.get_image();
    sf::Image source = image;

    for (unsigned id_x = 0; id_x < image.getSize().x; ++id_x) {
        for (unsigned id_y = 0; id_y < image.getSize().y; ++id_y) {
            // TODO: Consider selection

            image.setPixel(id_x, id_y, apply_at(source, id_x, id_y));
        }
    }
}
