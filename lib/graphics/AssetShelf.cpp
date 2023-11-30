#include "AssetShelf.h"

#include <string.h>

#include <SFML/Graphics/Image.hpp>

#include "sf_cheatsheet.hpp"

DesignDescriptor::DesignDescriptor() : image(0, 0) {}

void DesignDescriptor::load(const char* path) {
    static sf::Image loader;

    loader.loadFromFile(path);

    to_plug_texture(loader, image);
}

#define DESIGN(name, path) designs_[DSGN_##name].load(path);

sf::Font AssetShelf::font_;
DesignDescriptor AssetShelf::designs_[DESIGN_COUNT];

AssetShelf::AssetShelf() {
    font_.loadFromFile("assets/fonts/8bit.ttf");
#include "designs.hpp"
}

const sf::Font& AssetShelf::getFont() { return font_; }

DesignDescriptor& AssetShelf::getDesign(DesignId id) { return designs_[id]; }

AssetShelf& AssetShelf::getInstance() { return instance_; }

AssetShelf AssetShelf::instance_;
