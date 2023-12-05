#include "edge_detect.h"

#include <math.h>

EdgeFilter EdgeFilter::instance_ = EdgeFilter();
EdgeFilterData EdgeFilter::data_;

plug::Plugin* EdgeFilter::tryGetInterface(size_t interface_id) {
    if (interface_id != (size_t)plug::PluginGuid::Filter) return nullptr;

    return &instance_;
}

static const unsigned EDGE_THOLD = 6;

static unsigned char process_chanel(int value) {
    return (unsigned)abs(value) > EDGE_THOLD ? 255 : 0;
}

static plug::Color edge(plug::Canvas& canvas, size_t pos_x, size_t pos_y) {
    size_t lim_x = (size_t)canvas.getSize().x;
    size_t lim_y = (size_t)canvas.getSize().y;

    int red = 0, green = 0, blue = 0;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int multiplier = (dx == 0 && dy == 0) ? 8 : -1;

            long long cell_x = (long long)pos_x + dx;
            long long cell_y = (long long)pos_y + dy;
            if (cell_x >= (long long)lim_x || cell_x < 0)
                cell_x = (long long)pos_x;
            if (cell_y >= (long long)lim_y || cell_y < 0)
                cell_y = (long long)pos_y;

            plug::Color color = canvas.getPixel((size_t)cell_x, (size_t)cell_y);
            red += (int)color.r * multiplier;
            green += (int)color.g * multiplier;
            blue += (int)color.b * multiplier;
        }
    }

    red = abs(red) / 8;
    green = abs(green) / 8;
    blue = abs(blue) / 8;

    return plug::Color(process_chanel(red), process_chanel(green),
                       process_chanel(blue));
}

void EdgeFilter::applyFilter(plug::Canvas& canvas) const {
    size_t width = (size_t)canvas.getSize().x;
    size_t height = (size_t)canvas.getSize().y;

    plug::Color* new_data = new plug::Color[width * height];

    for (size_t pos_x = 0; pos_x < width; ++pos_x) {
        for (size_t pos_y = 0; pos_y < height; ++pos_y) {
            new_data[pos_x * height + pos_y] = edge(canvas, pos_x, pos_y);
        }
    }

    for (size_t pos_x = 0; pos_x < (size_t)canvas.getSize().x; ++pos_x) {
        for (size_t pos_y = 0; pos_y < (size_t)canvas.getSize().y; ++pos_y) {
            canvas.setPixel(pos_x, pos_y, new_data[pos_x * height + pos_y]);
        }
    }

    delete new_data;
}

plug::Plugin* loadPlugin(void) {
    static EdgeFilter instance;
    return &instance;
}
