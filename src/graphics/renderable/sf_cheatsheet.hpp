#ifndef SF_CHEATSHEET_HPP
#define SF_CHEATSHEET_HPP

#include <SFML/Graphics/Vertex.hpp>

#include "geometry/vector.hpp"

static inline Vec3d extrude(const Vec2d& vector) {
    return Vec3d(vector.get_x(), vector.get_y(), 1.0);
}

static inline sf::Vertex to_sf(const Vec3d& vector) {
    return sf::Vertex(sf::Vector2f((float)(vector.get_x() / vector.get_z()),
                                   (float)(vector.get_y() / vector.get_z())));
}

static inline sf::Vector2f to_Vector2f(const Vec3d& vector) {
    return sf::Vector2f((float)(vector.get_x() / vector.get_z()),
                        (float)(vector.get_y() / vector.get_z()));
}

static inline sf::Vector2f to_Vector2f(const Vec2d& vector) {
    return sf::Vector2f((float)(vector.get_x()), (float)(vector.get_y()));
}

static inline double clamp(double value) {
    return value < 0.0 ? 0.0 : (value > 1.0 ? 1.0 : value);
}

static inline sf::Color to_color(const Vec3d& color) {
    return sf::Color((sf::Uint8)(255 * clamp(color.get_x())),
                     (sf::Uint8)(255 * clamp(color.get_x())),
                     (sf::Uint8)(255 * clamp(color.get_x())));
}

#endif