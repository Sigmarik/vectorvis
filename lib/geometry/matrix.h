/**
 * @file matrix.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Matrix class
 * @version 0.1
 * @date 2023-09-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MATRIX_H
#define MATRIX_H

#include "data_structures/stack.h"
#include "vector.hpp"

struct Mat22d {
    explicit Mat22d();
    explicit Mat22d(double value);
    explicit Mat22d(double tl, double tr, double bl, double br);
    explicit Mat22d(const Vec2d& e1, const Vec2d& e2);
    ~Mat22d();

    Vec2d operator*(const Vec2d& vector) const;
    Mat22d operator*(const Mat22d& matrix) const;

   private:
    double tl_, tr_, bl_, br_;
};

struct Mat33d {
    explicit Mat33d();
    explicit Mat33d(double value);
    explicit Mat33d(double tl, double tm, double tr, double ml, double mm,
                    double mr, double bl, double bm, double br);
    explicit Mat33d(const Vec3d& e1, const Vec3d& e2, const Vec3d& e3);
    ~Mat33d();

    Vec3d operator*(const Vec3d& vector) const;
    Mat33d operator*(const Mat33d& matrix) const;

    double determinant() const;

    Mat33d inverse() const;

    double at(unsigned x, unsigned y) const;

   private:
    double tl_, tm_, tr_, ml_, mm_, mr_, bl_, bm_, br_;
};

#endif
