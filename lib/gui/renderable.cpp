#include "renderable.h"

#include <string.h>

void DesignDescriptor::load(const char* path) {
    static char file_path[1024] = "";

    size_t shift = strlen(path);

    strcpy(file_path, path);

    strcpy(file_path + shift, "/atlas.png");
    atlas.loadFromFile(file_path);

    strcpy(file_path + shift, "/shader.glsl");
    shader.loadFromFile(file_path, sf::Shader::Fragment);

    shader.setParameter("atlas", atlas);
}

#define DESIGN(name, path) designs[DSGN_##name].load(path);

AssetShelf::AssetShelf()
    : shaders_available(sf::Shader::isAvailable()), font() {
    font.loadFromFile("assets/fonts/main.ttf");
#include "designs.hpp"
}

#undef DESIGN
