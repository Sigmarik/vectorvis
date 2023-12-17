#include "AssetShelf.h"

#include <stdio.h>
#include <string.h>

#include <SFML/Graphics/Image.hpp>

#include "sf_cheatsheet.hpp"

DesignDescriptor::DesignDescriptor() : image(0, 0) {}

static bool path_exists(const char* path) {
    FILE* check = fopen(path, "r");
    if (check) {
        fclose(check);
        return true;
    }
    return false;
}

void DesignDescriptor::load(const char* path) {
    static sf::Image loader;

    if (path_exists(path)) {
        loader.loadFromFile(path);
    } else {
        loader.create(32, 32);
    }

    to_plug_texture(loader, image);
}

#define DESIGN(name, path) designs_[DSGN_##name].load(path);

sf::Font AssetShelf::font_;
DesignDescriptor AssetShelf::designs_[DESIGN_COUNT];

AssetShelf::AssetShelf() {
    const char FONT_PATH[] = "assets/fonts/8bit.ttf";
    if (path_exists(FONT_PATH)) font_.loadFromFile(FONT_PATH);
#include "designs.hpp"
}

const sf::Font& AssetShelf::getFont() { return font_; }

DesignDescriptor& AssetShelf::getDesign(DesignId id) { return designs_[id]; }

AssetShelf& AssetShelf::getInstance() { return instance_; }

AssetShelf AssetShelf::instance_;
