#include "../lib/geometry/matrix.h"

TEST(Matrix, Inverse) {
    Mat33d matrix(1.0, -2.0, 0.0, 3.0, 1.0, -1.0, -1.0, 0.0, 1.0);
    Vec3d test_vector(1.0, 2.0, 3.0);

    Mat33d unit_matrix = matrix * matrix.inverse();

    Vec3d result = unit_matrix * test_vector;

    EXPECT_FLOAT_EQ(result.get_x(), test_vector.get_x());
    EXPECT_FLOAT_EQ(result.get_y(), test_vector.get_y());
    EXPECT_FLOAT_EQ(result.get_z(), test_vector.get_z());
}
