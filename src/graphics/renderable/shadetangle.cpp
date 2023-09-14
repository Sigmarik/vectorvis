#include "shadetangle.h"

#include <string.h>

#include <SFML/Graphics/VertexArray.hpp>

#include "sf_cheatsheet.hpp"

Shadetangle::Shadetangle(shader_t* shader, const Vec2d& center,
                         const Vec2d& size, unsigned res_x, unsigned res_y)
    : shader_(shader),
      center_(center),
      size_(size),
      texture_(),
      texture_data_(
          (sf::Uint8*)calloc(res_x * res_y * 4, sizeof(*texture_data_))) {
    texture_.create(res_x, res_y);
}

Shadetangle::Shadetangle(const Shadetangle& shadetangle)
    : shader_(shadetangle.shader_),
      center_(shadetangle.center_),
      size_(shadetangle.size_),
      texture_(),
      texture_data_((sf::Uint8*)calloc(shadetangle.texture_.getSize().x *
                                           shadetangle.texture_.getSize().y * 4,
                                       sizeof(*texture_data_))) {
    memcpy(texture_data_, shadetangle.texture_data_,
           shadetangle.texture_.getSize().x * shadetangle.texture_.getSize().y *
               4);
    texture_.create(shadetangle.texture_.getSize().x,
                    shadetangle.texture_.getSize().y);
}

Shadetangle::~Shadetangle() { free(texture_data_); }

Shadetangle& Shadetangle::operator=(const Shadetangle& shadetangle) {
    shader_ = shadetangle.shader_;
    center_ = shadetangle.center_;
    size_ = shadetangle.size_;
    texture_ = sf::Texture();
    texture_data_ = (sf::Uint8*)calloc(
        shadetangle.texture_.getSize().x * shadetangle.texture_.getSize().y * 4,
        sizeof(*texture_data_));
    memcpy(texture_data_, shadetangle.texture_data_,
           shadetangle.texture_.getSize().x * shadetangle.texture_.getSize().y *
               4);
    texture_.create(shadetangle.texture_.getSize().x,
                    shadetangle.texture_.getSize().y);
    return *this;
}

void Shadetangle::render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                         const AssetShelf& assets) {
    static sf::Vertex vertices[4];

    texture_.setSmooth(true);

    Vec2d half_size = size_ / 2.0;
    Vec2d half_size_inv = size_ / 2.0;
    half_size_inv.set_x(-half_size_inv.get_x());

    vertices[0] = to_sf(stack.top() * extrude(center_ + half_size_inv));
    vertices[1] = to_sf(stack.top() * extrude(center_ + half_size));
    vertices[2] = to_sf(stack.top() * extrude(center_ - half_size_inv));
    vertices[3] = to_sf(stack.top() * extrude(center_ - half_size));

    vertices[0].texCoords = sf::Vector2f(0.0, 0.0);
    vertices[1].texCoords = sf::Vector2f((float)texture_.getSize().x, 0.0);
    vertices[2].texCoords =
        sf::Vector2f((float)texture_.getSize().x, (float)texture_.getSize().y);
    vertices[3].texCoords = sf::Vector2f(0.0, (float)texture_.getSize().y);

    apply_shader();

    // target.draw(vertices, sf::RenderStates(&texture_));
    target.draw(vertices, sizeof(vertices) / sizeof(*vertices),
                sf::PrimitiveType::Quads, sf::RenderStates(&texture_));
}

shader_t* Shadetangle::get_shader() const { return shader_; }

Vec2d Shadetangle::get_center() const { return center_; }

Vec2d Shadetangle::get_size() const { return size_; }

void Shadetangle::set_shader(shader_t* shader) { shader_ = shader; }

void Shadetangle::set_center(const Vec2d& center) { center_ = center; }

void Shadetangle::set_size(const Vec2d& size) { size_ = size; }

void Shadetangle::apply_shader() {
    Vec2d uv;
    Vec2d size = get_size();
    for (unsigned x_id = 0; x_id < texture_.getSize().x; ++x_id) {
        for (unsigned y_id = 0; y_id < texture_.getSize().y; ++y_id) {
            uv.set_x(-1.0 + 2.0 * x_id / texture_.getSize().x);
            uv.set_y(1.0 - 2.0 * y_id / texture_.getSize().y);

            sf::Color color = shader_(uv, size);

            unsigned id = (y_id * texture_.getSize().y + x_id) * 4;
            texture_data_[id + 0] = color.r;
            texture_data_[id + 1] = color.g;
            texture_data_[id + 2] = color.b;
            texture_data_[id + 3] = color.a;
        }
    }
    texture_.update(texture_data_);
}
