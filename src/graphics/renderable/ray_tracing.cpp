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

#define FOR_EACH_SHAPE(code)                                                  \
    {                                                                         \
        for (size_t sphere_id = 0; sphere_id < spheres.size(); ++sphere_id) { \
            Sphere shape = spheres[sphere_id];                                \
            code                                                              \
        }                                                                     \
        for (size_t plane_id = 0; plane_id < planes.size(); ++plane_id) {     \
            Plane shape = planes[plane_id];                                   \
            code                                                              \
        }                                                                     \
    }

HitResult Scene::trace(const Vec3d& start, const Vec3d& direction) const {
    HitResult hit = {.valid = false,
                     .position = Vec3d(999, 999, 999),
                     .normal = Vec3d(0.0, 0.0, 1.0),
                     .material = {}};
    double depth = INFINITY;

    FOR_EACH_SHAPE({
        HitResult local = shape.check(start, direction);
        double local_depth = (local.position - start).length2();

        if (!hit.valid || (local.valid && local_depth < depth)) {
            depth = local_depth;
            hit = local;
        }
    })

    return hit;
}

static const double SKY_BRIGHTNESS = 0.27;
static const Color SKY_LIGHT(0.9 * SKY_BRIGHTNESS, 1.0 * SKY_BRIGHTNESS,
                             1.0 * SKY_BRIGHTNESS);

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

static double brdf(const Vec3d& light, const Vec3d& normal, const Vec3d& eye,
                   const Material& material) {
    Vec3d bisector = -(light + eye).normalize();
    double diffuse = MAX(0.0, dot(-light, normal));
    double specular =
        pow(MAX(0.0, dot(bisector, normal)), 1.0 / material.roughness);
    return specular * material.specular + diffuse;
}

static const unsigned SUBRAY_COUNT = 1;

static double rand_double() {
    double source = sin(rand()) * 1024.0;
    return source - floor(source);
}

Color Scene::measure(const Vec3d& start, const Vec3d& direction,
                     unsigned depth) const {
    HitResult hit = trace(start, direction);

    if (!hit.valid) {
        return SKY_LIGHT;
    }

    if (hit.material.mirror && depth > 0) {
        return measure(hit.position + hit.normal * 0.01,
                       direction - hit.normal * dot(direction, hit.normal) *
                                       2.0 / hit.normal.length2(),
                       depth - 1) *
               hit.material.color;
    }

    Color result = SKY_LIGHT * hit.material.color;

    for (size_t light_id = 0; light_id < lights.size(); ++light_id) {
        PointLight light = lights[light_id];
        Vec3d light_vector = (hit.position - light.position).normalize();

        HitResult obstruct =
            trace(hit.position + hit.normal * 0.01, -light_vector);

        if (obstruct.valid && (obstruct.position - hit.position).length2() <
                                  (light.position - hit.position).length2()) {
            continue;
        }

        result += (light.color / (light.position - hit.position).length2()) *
                  hit.material.color *
                  brdf(light_vector, hit.normal,
                       (hit.position - start).normalize(), hit.material);
    }

    if (depth == 0) return result;

    for (unsigned ray_id = 0; ray_id < SUBRAY_COUNT; ++ray_id) {
        Vec3d other_axis = cross(hit.normal, Vec3d(rand_double(), rand_double(),
                                                   rand_double()))
                               .normalize();
        Vec3d another_axis = cross(hit.normal, other_axis).normalize();
        Vec3d ray = (hit.normal * rand_double() +
                     other_axis * (rand_double() * 2.0 - 1.0) +
                     another_axis * (rand_double() * 2.0 - 1.0))
                        .normalize();
        Color subray_color =
            measure(hit.position + hit.normal * 0.01, ray, depth - 1);
        result += subray_color * hit.material.color *
                  brdf(-ray, hit.normal, (hit.position - start).normalize(),
                       hit.material) /
                  SUBRAY_COUNT;
    }

    return result;
}

double smoothstep(double parameter) {
    if (parameter < 0.0) return 0.0;
    if (parameter > 1.0) return 1.0;
    return 3.0 * parameter * parameter -
           2.0 * parameter * parameter * parameter;
}
