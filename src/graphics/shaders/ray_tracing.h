/**
 * @file shapes.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Ray tracing primitives
 * @version 0.1
 * @date 2023-09-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef RAY_TRACING_H
#define RAY_TRACING_H

#include "geometry/vector.h"

typedef Vec3d Color;

struct Material {
    Color color = Color(1.0, 1.0, 1.0);
    double roughness = 1.0, specular = 0.5;
    bool mirror = false;
};

struct HitResult {
    bool valid = false;
    Vec3d position, normal;
    Material material;
};

struct PointLight {
    Vec3d position;
    Color color;
};

struct Shape {
    virtual ~Shape() = default;
    virtual HitResult check(const Vec3d& start,
                            const Vec3d& direction) const = 0;

   protected:
    Material material_ = {};
};

struct Sphere final : public Shape {
    ~Sphere() = default;

    Sphere(Vec3d center, double radius, Material material);

    HitResult check(const Vec3d& start, const Vec3d& direction) const override;

   private:
    Vec3d center_;
    double radius_;
};

struct Plane final : public Shape {
    ~Plane() = default;

    Plane(const Vec3d& center, const Vec3d& normal, Material material);

    HitResult check(const Vec3d& start, const Vec3d& direction) const override;

   private:
    Vec3d center_;
    Vec3d normal_;
};

double smoothstep(double parameter);

#endif
