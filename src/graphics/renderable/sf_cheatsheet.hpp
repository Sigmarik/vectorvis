#ifndef SF_CHEATSHEET_HPP
#define SF_CHEATSHEET_HPP

#include <SFML/Graphics/Vertex.hpp>

#include "geometry/vector.h"

static inline Vec3d extrude(const Vec2d& vector) {
    return Vec3d(vector.get_x(), vector.get_y(), 1.0);
}

static inline sf::Vertex to_sf(const Vec3d& vector) {
    return sf::Vertex(sf::Vector2f((float)(vector.get_x() / vector.get_z()),
                                   (float)(vector.get_y() / vector.get_z())));
}

#endif