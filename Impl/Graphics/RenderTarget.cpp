#include "RenderTarget.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

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

static void to_sf_image(const plug::Texture& data, sf::Image& image) {
    image.create((unsigned)data.width, (unsigned)data.height);

    for (unsigned id_x = 0; id_x < data.width; ++id_x) {
        for (unsigned id_y = 0; id_y < data.height; ++id_y) {
            size_t index = id_y * data.height + id_x;

            plug::Color pixel = data.data[index];

            image.setPixel(id_x, id_y,
                           sf::Color(pixel.r, pixel.g, pixel.b, pixel.a));
        }
    }
}

void RenderTarget::draw(const plug::VertexArray& array,
                        const plug::Texture& texture) {
    sf::Image sf_image;
    to_sf_image(texture, sf_image);

    // sf::Image image = sf_texture.copyToImage();

    // for (unsigned id = 0; id < 32; ++id) {
    //     printf("%u ", (unsigned)image.getPixelsPtr()[id * 4 + 1]);
    // }
    // putc('\n', stdout);

    sf::VertexArray sf_array = to_sf_vertex_array(array);

    for (size_t id = 0; id < sf_array.getVertexCount(); ++id) {
        sf_array[id].texCoords =
            sf::Vector2f(sf_array[id].texCoords.x * (float)texture.width,
                         sf_array[id].texCoords.y * (float)texture.height);
    }

    static sf::Texture sf_texture;
    sf_texture.loadFromImage(sf_image);

    base_.draw(sf_array, &sf_texture);
}

void RenderTarget::clear(plug::Color color) { base_.clear(); }

void RenderTarget::setActive(bool active) {
    if (active) base_.pushGLStates();

    base_.setActive(active);

    if (active) base_.resetGLStates();

    if (!active) base_.popGLStates();
}
