#ifndef SECURE_VECTOR_TESTS_HPP
#define SECURE_VECTOR_TESTS_HPP

#include <math.h>

#include "../lib/geometry/secure_vector.h"

TEST(SecureVector2d, Constructors) {
    SecureVec2d vector;

    EXPECT_EQ(vector.get_x(), 0.0);
    EXPECT_EQ(vector.get_y(), 0.0);

    vector = SecureVec2d(69.0, 420.0);
    EXPECT_EQ(vector.get_x(), 69.0);
    EXPECT_EQ(vector.get_y(), 420.0);

    vector = SecureVec2d(M_PI / 2.0);
    EXPECT_LE(abs(vector.get_x()), 1e-10);
    EXPECT_LE(abs(vector.get_y() - 1.0), 1e-10);

    vector = SecureVec2d::radial(M_PI / 2.0, 2.5);
    EXPECT_LE(abs(vector.get_x()), 1e-10);
    EXPECT_LE(abs(vector.get_y() - 2.5), 1e-10);
}

TEST(SecureVector2d, Operators) {
    SecureVec2d alpha(1.0, 2.0);
    SecureVec2d beta(3.0, 4.0);

    SecureVec2d sum = alpha + beta;
    SecureVec2d dif = alpha - beta;
    SecureVec2d double_alpha = alpha * 2.0;
    SecureVec2d half_alpha = alpha / 2.0;

    EXPECT_EQ(alpha, SecureVec2d(1.0, 2.0));
    EXPECT_EQ(beta, SecureVec2d(3.0, 4.0));

    EXPECT_EQ(sum, SecureVec2d(4.0, 6.0));
    EXPECT_EQ(dif, SecureVec2d(-2.0, -2.0));

    EXPECT_EQ(double_alpha, SecureVec2d(2.0, 4.0));
    EXPECT_EQ(half_alpha, SecureVec2d(0.5, 1.0));

    alpha += SecureVec2d(1.0, 3.0);
    EXPECT_EQ(alpha, SecureVec2d(2.0, 5.0));

    alpha -= SecureVec2d(1.0, 3.0);
    EXPECT_EQ(alpha, SecureVec2d(1.0, 2.0));

    alpha *= 2.0;
    EXPECT_EQ(alpha, SecureVec2d(2.0, 4.0));

    alpha /= 2.0;
    EXPECT_EQ(alpha, SecureVec2d(1.0, 2.0));

    EXPECT_EQ(-alpha, SecureVec2d(-1.0, -2.0));
}

TEST(SecureVector2d, Characteristics) {
    EXPECT_EQ(SecureVec2d(3.0, 4.0).length(), 5.0);
    EXPECT_EQ(SecureVec2d(4.0, 3.0).length2(), 25.0);

    EXPECT_EQ(SecureVec2d(0.0, 0.1).azimuth(), M_PI / 2.0);

    EXPECT_EQ(SecureVec2d::radial(M_PI, 0.5).angle(
                  SecureVec2d::radial(M_PI / 2.0, 0.1)),
              -M_PI / 2.0);
    EXPECT_EQ(dot(SecureVec2d(1.0, 1.0), SecureVec2d(1.0, 2.0)), 3.0);
    EXPECT_EQ(cross(SecureVec2d(1.0, 2.0), SecureVec2d(3.0, 4.0)), -2.0);
}

TEST(SecureVector2d, Transforms) {
    EXPECT_EQ(SecureVec2d(3.0, 4.0).normalize(),
              SecureVec2d(3.0 / 5.0, 4.0 / 5.0));
    EXPECT_EQ(SecureVec2d(0.0, 0.0).normalize(), SecureVec2d(0.0, 0.0));

    EXPECT_EQ(SecureVec2d(3.0, 4.0).project_on_axis(SecureVec2d(10.0, 0.0)),
              SecureVec2d(3.0, 0.0));
    EXPECT_EQ(SecureVec2d(3.0, 4.0).project_on_plane(SecureVec2d(10.0, 0.0)),
              SecureVec2d(0.0, 4.0));

    EXPECT_EQ(SecureVec2d(3.0, 4.0).left(), SecureVec2d(-4.0, 3.0));
    EXPECT_EQ(SecureVec2d(3.0, 4.0).right(), SecureVec2d(4.0, -3.0));
    EXPECT_EQ(SecureVec2d(3.0, 4.0).back(), SecureVec2d(-3.0, -4.0));
}

#endif
