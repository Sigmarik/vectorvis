#include "arrow.h"

#include <stdio.h>

#include <SFML/Graphics/Vertex.hpp>

#include "sf_cheatsheet.hpp"

Arrow::Arrow() : vector_(0.0, 0.0) {}

Arrow::Arrow(const Vec2d& vector) : vector_(vector) {}

Arrow::~Arrow() {}

void Arrow::render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                   const AssetShelf& assets) {
    static sf::Vertex vertices[6];

    if (vector_.length2() < 1e-7) return;

    Mat33d projection = stack.top();

    Vec3d start = projection * Vec3d(0.0, 0.0, 1.0);
    Vec3d end = projection * extrude(vector_);

    const double ARROW_SIZE = 0.05;

    Vec3d left_wing = projection * extrude(vector_ * (1.0 - ARROW_SIZE) +
                                           vector_.left() * ARROW_SIZE);
    Vec3d right_wing = projection * extrude(vector_ * (1.0 - ARROW_SIZE) +
                                            vector_.right() * ARROW_SIZE);

    vertices[0] = to_sf(start);
    vertices[1] = to_sf(end);
    vertices[2] = to_sf(end);
    vertices[3] = to_sf(left_wing);
    vertices[4] = to_sf(end);
    vertices[5] = to_sf(right_wing);

    target.draw(vertices, sizeof(vertices) / sizeof(*vertices), sf::Lines);
}

Vec2d Arrow::get_vector() { return Vec2d(vector_.get_x(), vector_.get_y()); }

void Arrow::set_vector(const Vec2d& vector) { vector_ = vector; }
