#include "shaders.h"

SHADER(basic_shaders::white) { return sf::Color(255, 255, 255); }

SHADER(basic_shaders::uv) {
    return sf::Color((sf::Uint8)(int)((0.5 + 0.5 * uv.get_x()) * 255.0), 0,
                     (sf::Uint8)(int)((0.5 + 0.5 * uv.get_y()) * 255.0));
}
