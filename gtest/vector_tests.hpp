#ifndef VECTOR_TESTS_HPP
#define VECTOR_TESTS_HPP

#include <math.h>

#include "../lib/geometry/vector.h"

TEST(Vector2d, Constructors) {
    Vec2d vector;

    EXPECT_EQ(vector.get_x(), 0.0);
    EXPECT_EQ(vector.get_y(), 0.0);

    vector = Vec2d(69.0, 420.0);
    EXPECT_EQ(vector.get_x(), 69.0);
    EXPECT_EQ(vector.get_y(), 420.0);

    vector = Vec2d(M_PI / 2.0);
    EXPECT_LE(abs(vector.get_x()), 1e-10);
    EXPECT_LE(abs(vector.get_y() - 1.0), 1e-10);

    vector = Vec2d::radial(M_PI / 2.0, 2.5);
    EXPECT_LE(abs(vector.get_x()), 1e-10);
    EXPECT_LE(abs(vector.get_y() - 2.5), 1e-10);
}

TEST(Vector2d, Operators) {
    Vec2d alpha(1.0, 2.0);
    Vec2d beta(3.0, 4.0);

    Vec2d sum = alpha + beta;
    Vec2d dif = alpha - beta;
    Vec2d double_alpha = alpha * 2.0;
    Vec2d half_alpha = alpha / 2.0;

    EXPECT_EQ(alpha, Vec2d(1.0, 2.0));
    EXPECT_EQ(beta, Vec2d(3.0, 4.0));

    EXPECT_EQ(sum, Vec2d(4.0, 6.0));
    EXPECT_EQ(dif, Vec2d(-2.0, -2.0));

    EXPECT_EQ(double_alpha, Vec2d(2.0, 4.0));
    EXPECT_EQ(half_alpha, Vec2d(0.5, 1.0));

    alpha += Vec2d(1.0, 3.0);
    EXPECT_EQ(alpha, Vec2d(2.0, 5.0));

    alpha -= Vec2d(1.0, 3.0);
    EXPECT_EQ(alpha, Vec2d(1.0, 2.0));

    alpha *= 2.0;
    EXPECT_EQ(alpha, Vec2d(2.0, 4.0));

    alpha /= 2.0;
    EXPECT_EQ(alpha, Vec2d(1.0, 2.0));

    EXPECT_EQ(-alpha, Vec2d(-1.0, -2.0));
}

TEST(Vector2d, Characteristics) {
    EXPECT_EQ(Vec2d(3.0, 4.0).length(), 5.0);
    EXPECT_EQ(Vec2d(4.0, 3.0).length2(), 25.0);

    EXPECT_EQ(Vec2d(0.0, 0.1).azimuth(), M_PI / 2.0);

    EXPECT_EQ(Vec2d::radial(M_PI, 0.5).angle(Vec2d::radial(M_PI / 2.0, 0.1)),
              -M_PI / 2.0);
    EXPECT_EQ(dot(Vec2d(1.0, 1.0), Vec2d(1.0, 2.0)), 3.0);
    EXPECT_EQ(cross(Vec2d(1.0, 2.0), Vec2d(3.0, 4.0)), -2.0);
}

TEST(Vector2d, Transforms) {
    EXPECT_EQ(Vec2d(3.0, 4.0).normalize(), Vec2d(3.0 / 5.0, 4.0 / 5.0));
    EXPECT_EQ(Vec2d(0.0, 0.0).normalize(), Vec2d(0.0, 0.0));

    EXPECT_EQ(Vec2d(3.0, 4.0).project_on_axis(Vec2d(10.0, 0.0)),
              Vec2d(3.0, 0.0));
    EXPECT_EQ(Vec2d(3.0, 4.0).project_on_plane(Vec2d(10.0, 0.0)),
              Vec2d(0.0, 4.0));

    EXPECT_EQ(Vec2d(3.0, 4.0).left(), Vec2d(-4.0, 3.0));
    EXPECT_EQ(Vec2d(3.0, 4.0).right(), Vec2d(4.0, -3.0));
    EXPECT_EQ(Vec2d(3.0, 4.0).back(), Vec2d(-3.0, -4.0));
}

#endif
