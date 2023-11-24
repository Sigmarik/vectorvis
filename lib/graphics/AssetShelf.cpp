#include "AssetShelf.h"

#include <string.h>

#include <SFML/Graphics/Image.hpp>

DesignDescriptor::DesignDescriptor() : image(0, 0) {
    delete image.data;
    image.width = 0;
    image.height = 0;
}

void DesignDescriptor::load(const char* path) {
    static sf::Image loader;

    loader.loadFromFile(path);

    image = plug::Texture(loader.getSize().x, loader.getSize().y);

    image.width = loader.getSize().x;
    image.height = loader.getSize().y;
    image.data = new plug::Color[image.width * image.height];
    const sf::Uint8* data = loader.getPixelsPtr();

    for (unsigned id_x = 0; id_x < image.width; ++id_x) {
        for (unsigned id_y = 0; id_y < image.height; ++id_y) {
            size_t index = id_y * image.height + id_x;
            sf::Color pixel = loader.getPixel(id_x, id_y);
            image.data[index] = plug::Color(pixel.r, pixel.g, pixel.b, pixel.a);
        }
    }
}

#define DESIGN(name, path) designs_[DSGN_##name].load(path);

sf::Font AssetShelf::font_;
DesignDescriptor AssetShelf::designs_[DESIGN_COUNT];

AssetShelf::AssetShelf() {
    font_.loadFromFile("assets/fonts/main.ttf");
#include "designs.hpp"
}

const sf::Font& AssetShelf::getFont() { return font_; }

DesignDescriptor& AssetShelf::getDesign(DesignId id) { return designs_[id]; }

AssetShelf& AssetShelf::getInstance() { return instance_; }

AssetShelf AssetShelf::instance_;
