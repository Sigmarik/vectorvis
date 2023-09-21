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

AssetShelf::AssetShelf()
    : shaders_available(sf::Shader::isAvailable()),
      font(),
      button_design(),
      panel_design() {
    font.loadFromFile("assets/fonts/main.ttf");
    button_design.load("assets/gui/button");
    panel_design.load("assets/gui/panel");
}
