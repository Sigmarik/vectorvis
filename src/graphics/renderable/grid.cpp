#include "grid.h"

#include <math.h>

Grid::Grid(const Vec2d& x_unit, const Vec2d& y_unit)
    : x_unit_(x_unit), y_unit_(y_unit) {}

static const unsigned MARK_COUNT = 10;
static const unsigned GRID_POWER = 3;

void Grid::render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target) {
    static sf::Vertex vertices[4 + 2 * 2 * MARK_COUNT * GRID_POWER];

    Vec3d x_start(-pow(10, GRID_POWER - 1), 0.0, 1.0);
}

void Grid::set_x_unit(const Vec2d& vector) {}

void Grid::set_y_unit(const Vec2d& vector) {}

Vec2d Grid::get_x_unit() const { return Vec2d(); }

Vec2d Grid::get_y_unit() const { return Vec2d(); }

Mat33d Grid::matrix() { return Mat33d(); }
