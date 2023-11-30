#include "RenderTarget.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "graphics/sf_cheatsheet.hpp"

static sf::Vector2f to_vector2f(const Vec2d& vector) {
    return sf::Vector2f((float)vector.x, (float)vector.y);
}

static sf::Color to_sf_color(const plug::Color& color) {
    return sf::Color(color.r, color.g, color.b, color.a);
}

static sf::VertexArray to_sf_vertex_array(const plug::VertexArray& array) {
    sf::VertexArray vertices((sf::PrimitiveType)array.getPrimitive(),
                             array.getSize());

    for (size_t id = 0; id < array.getSize(); ++id) {
        vertices[id].position = to_vector2f(array[id].position);
        vertices[id].texCoords = to_vector2f(array[id].tex_coords);
        vertices[id].color = to_sf_color(array[id].color);
    }

    return vertices;
}

void RenderTarget::draw(const plug::VertexArray& array) {
    base_.draw(to_sf_vertex_array(array));
}

void RenderTarget::draw(const plug::VertexArray& array,
                        const plug::Texture& texture) {
    sf::Image sf_image;

    fflush(stdout);

    to_sf_image(texture, sf_image);

    sf::VertexArray sf_array = to_sf_vertex_array(array);

    for (size_t id = 0; id < sf_array.getVertexCount(); ++id) {
        sf_array[id].texCoords =
            sf::Vector2f(sf_array[id].texCoords.x * (float)texture.width,
                         sf_array[id].texCoords.y * (float)texture.height);
    }

    static sf::Texture sf_texture;
    sf_texture.loadFromImage(sf_image);
    sf_texture.setRepeated(false);

    base_.draw(sf_array, &sf_texture);
}

void RenderTarget::clear(plug::Color color) { base_.clear(); }

void RenderTarget::setActive(bool active) {
    if (active) base_.pushGLStates();

    base_.setActive(active);

    if (active) base_.resetGLStates();

    if (!active) base_.popGLStates();
}
