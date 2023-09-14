/**
 * @file scene_shader.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Rectangle with CPU-computed shader
 * @version 0.1
 * @date 2023-09-09
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SHADETANGLE_H
#define SHADETANGLE_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "renderable.h"

#define SHADER(name) sf::Color name(const Vec2d& uv, const Vec2d& size)
typedef SHADER(shader_t);

struct Shadetangle : public Renderable {
    explicit Shadetangle(shader_t* shader, const Vec2d& center,
                         const Vec2d& size, unsigned res_x = 512,
                         unsigned res_y = 512);
    explicit Shadetangle(const Shadetangle& shadetangle);
    ~Shadetangle();

    Shadetangle& operator=(const Shadetangle& shadetangle);

    void render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target, const AssetShelf& assets) override;

    shader_t* get_shader() const;
    Vec2d get_center() const;
    Vec2d get_size() const;

    void set_shader(shader_t* shader);
    void set_center(const Vec2d& center);
    void set_size(const Vec2d& size);

   private:
    void apply_shader();

    shader_t* shader_;
    Vec2d center_, size_;
    sf::Texture texture_;
    sf::Uint8* texture_data_;
};

#endif
