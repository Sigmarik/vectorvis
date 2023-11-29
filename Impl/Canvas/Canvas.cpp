#include "Canvas.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "graphics/sf_cheatsheet.hpp"

void Canvas::synchGPU() const {
    if (!cpu_sided_) return;

    cpu_sided_ = false;

    static sf::VertexArray vertices(sf::PrimitiveType::Quads, 4);

    unsigned width = cpu_content_.getSize().x;
    unsigned height = cpu_content_.getSize().y;

    vertices[0].position = sf::Vector2f(0.0, 0.0);
    vertices[1].position = sf::Vector2f((float)width, 0.0);
    vertices[2].position = sf::Vector2f((float)width, (float)height);
    vertices[3].position = sf::Vector2f(0.0, (float)height);

    vertices[0].texCoords = vertices[0].position;
    vertices[1].texCoords = vertices[1].position;
    vertices[2].texCoords = vertices[2].position;
    vertices[3].texCoords = vertices[3].position;

    static sf::Texture texture;

    texture.loadFromImage(cpu_content_);

    gpu_content_.draw(vertices, sf::RenderStates(&texture));
}

void Canvas::synchCPU() const {
    if (cpu_sided_) return;
    cpu_sided_ = true;

    gpu_content_.display();
    sf::Image image = gpu_content_.getTexture().copyToImage();
    cpu_content_ = image;
}

Canvas::Canvas(size_t width, size_t height) : mask_(width, height) {
    gpu_content_.create((unsigned)width, (unsigned)height);
    cpu_content_.create((unsigned)width, (unsigned)height);
}

Canvas::Canvas(const plug::Texture& texture) : mask_() {
    gpu_content_.create((unsigned)texture.width, (unsigned)texture.height);
    to_sf_image(texture, cpu_content_);
    mask_.build(texture.width, texture.height);
}

Canvas::Canvas(const char* path) : mask_() {
    cpu_content_.loadFromFile(path);

    unsigned width = cpu_content_.getSize().x;
    unsigned height = cpu_content_.getSize().y;

    gpu_content_.create(width, height);
    mask_.build(width, height);
}

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

void Canvas::draw(const plug::VertexArray& vertex_array) {
    synchGPU();
    gpu_content_.draw(to_sf_vertex_array(vertex_array));
}

void Canvas::draw(const plug::VertexArray& vertex_array,
                  const plug::Texture& texture) {
    synchGPU();
    sf::Image sf_image;

    fflush(stdout);

    to_sf_image(texture, sf_image);

    sf::VertexArray sf_array = to_sf_vertex_array(vertex_array);

    for (size_t id = 0; id < sf_array.getVertexCount(); ++id) {
        sf_array[id].texCoords =
            sf::Vector2f(sf_array[id].texCoords.x * (float)texture.width,
                         sf_array[id].texCoords.y * (float)texture.height);
    }

    static sf::Texture sf_texture;
    sf_texture.loadFromImage(sf_image);

    gpu_content_.draw(sf_array, &sf_texture);
}

void Canvas::setSize(const plug::Vec2d& size) {
    // TODO: Implement
    printf("Nope.\n");
}

plug::Color Canvas::getPixel(size_t x, size_t y) const {
    synchCPU();

    sf::Color color = cpu_content_.getPixel((unsigned)x, (unsigned)y);

    return plug::Color(color.r, color.g, color.b, color.a);
}

void Canvas::setPixel(size_t x, size_t y, const plug::Color& color) {
    synchCPU();

    cpu_content_.setPixel((unsigned)x, (unsigned)y,
                          sf::Color(color.r, color.g, color.b, color.a));
}

const plug::Texture& Canvas::getTexture(void) const {
    synchCPU();

    static plug::Texture texture(128, 128);

    to_plug_texture(cpu_content_, texture);

    return texture;
}
