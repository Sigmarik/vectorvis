#include "ray_tracing.h"

#include <math.h>

Sphere::Sphere(Vec3d center, double radius, Material material)
    : center_(center), radius_(radius) {
    material_ = material;
}

HitResult Sphere::check(const Vec3d& start, const Vec3d& direction) const {
    Vec3d relative_pos = center_ - start;

    if (relative_pos.length2() < radius_ * radius_) {
        return (HitResult){
            .valid = true, .position = start, .normal = (start - center_)};
    }
    double dot_product = dot(direction, relative_pos);
    Vec3d projection = direction * dot_product / direction.length2();
    double distance = (projection - relative_pos).length();

    if (dot_product < 0.0 || distance > radius_) {
        return (HitResult){.valid = false,
                           .position = start + projection,
                           .normal = Vec3d(0.0, 0.0, 1.0),
                           .material = material_};
    }

    projection -=
        direction.normalize() * sqrt(radius_ * radius_ - distance * distance);

    return (HitResult){.valid = true,
                       .position = start + projection,
                       .normal = (projection - relative_pos).normalize(),
                       .material = material_};
}

Plane::Plane(const Vec3d& center, const Vec3d& normal, Material material)
    : center_(center), normal_(normal.normalize()) {
    material_ = material;
}

HitResult Plane::check(const Vec3d& start, const Vec3d& direction) const {
    Vec3d relative_pos = center_ - start;

    double dot_product = dot(direction, -normal_);
    double distance = dot(relative_pos, -normal_);

    if (dot_product <= 0.0) {
        return (HitResult){.valid = false,
                           .position = Vec3d(),
                           .normal = normal_,
                           .material = material_};
    }

    return (HitResult){
        .valid = true,
        .position = start + direction.normalize() * distance / dot_product,
        .normal = normal_,
        .material = material_};
}

double smoothstep(double parameter) {
    if (parameter < 0.0) return 0.0;
    if (parameter > 1.0) return 1.0;
    return 3.0 * parameter * parameter -
           2.0 * parameter * parameter * parameter;
}
