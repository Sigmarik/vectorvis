#include "rt_scene.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "ray_tracing.h"

static const unsigned LD_RESOLUTION = 1024;

static unsigned Iteration = 1;
static double LightData[(LD_RESOLUTION + 1) * (LD_RESOLUTION + 1) * 3] = {};

static const PointLight LIGHTS[] = {
    (PointLight){.position = Vec3d(0.0, 5.6, 10.0),
                 .color = Color(1.0, 1.0, 1.0) * 10.0},
    (PointLight){.position = Vec3d(5.0, -5.0, 15.0),
                 .color = Color(1.0, 0.1, 0.1) * 5.0}};

static const Sphere SPHERES[] = {
    Sphere(
        Vec3d(0.0, 0.0, 10.0), 2.0,
        (Material){
            .color = Vec3d(0.9, 0.9, 0.9), .roughness = 0.1, .specular = 1.0}),
    Sphere(Vec3d(-3.0, -3.0, 13.0), 2.0,
           (Material){.color = Vec3d(0.8, 0.8, 0.86),
                      .roughness = 0.01,
                      .specular = 1.0,
                      .mirror = true})};

static const Plane PLANES[] = {
    Plane(
        Vec3d(0.0, 6.0, 10.0), Vec3d(0.0, -1.0, 0.0),
        (Material){
            .color = Vec3d(1.0, 1.0, 1.0), .roughness = 0.1, .specular = 1.0}),
    Plane(
        Vec3d(0.0, -6.0, 10.0), Vec3d(0.0, 1.0, 0.0),
        (Material){
            .color = Vec3d(1.0, 1.0, 1.0), .roughness = 0.1, .specular = 1.0}),
    Plane(
        Vec3d(0.0, 0.0, 16.0), Vec3d(0.0, 0.0, -1.0),
        (Material){
            .color = Vec3d(1.0, 1.0, 1.0), .roughness = 0.1, .specular = 0.0}),
    Plane(
        Vec3d(-6.0, 0.0, 10.0), Vec3d(1.0, 0.0, 0.0),
        (Material){
            .color = Vec3d(1.0, 0.1, 0.1), .roughness = 0.1, .specular = 1.0}),
    Plane(
        Vec3d(6.0, 0.0, 10.0), Vec3d(-1.0, 0.0, 0.0),
        (Material){
            .color = Vec3d(0.1, 1.0, 0.1), .roughness = 0.1, .specular = 1.0}),
    Plane(
        Vec3d(0.0, 0.0, -1.0), Vec3d(0.0, 0.0, 1.0),
        (Material){
            .color = Vec3d(0.1, 0.1, 1.0), .roughness = 0.1, .specular = 1.0}),
};

#define LEN(array) (sizeof(array) / sizeof(*array))

#define FOR_EACH_SHAPE(code)                                                \
    {                                                                       \
        for (size_t sphere_id = 0; sphere_id < LEN(SPHERES); ++sphere_id) { \
            Sphere shape = SPHERES[sphere_id];                              \
            code                                                            \
        }                                                                   \
        for (size_t plane_id = 0; plane_id < LEN(PLANES); ++plane_id) {     \
            Plane shape = PLANES[plane_id];                                 \
            code                                                            \
        }                                                                   \
    }

static HitResult trace(const Vec3d& start, const Vec3d& direction) {
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

static const double SKY_BRIGHTNESS = 0.5;
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
    return diffuse + specular * material.specular;
}

static const unsigned SUBRAY_COUNT = 1;

static double rand_double() {
    double source = sin(rand());
    return source - floor(source);
}

static Color measure(Vec3d start, Vec3d direction, unsigned depth = 1) {
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

    for (size_t light_id = 0; light_id < LEN(LIGHTS); ++light_id) {
        PointLight light = LIGHTS[light_id];
        Vec3d light_vector = (hit.position - light.position).normalize();

        if (dot(hit.normal, -light_vector) < 0.0) continue;

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

SHADER(rt_example) {
    unsigned ld_x = (unsigned)((0.5 + 0.5 * uv.get_x()) * LD_RESOLUTION);
    unsigned ld_y = (unsigned)((0.5 + 0.5 * uv.get_y()) * LD_RESOLUTION);
    unsigned ld_id = (ld_y * LD_RESOLUTION + ld_x) * 3;

    if (ld_x <= 5 && ld_y <= 5) ++Iteration;

    if (Iteration == 1) {
        LightData[ld_id + 0] = 0.0;
        LightData[ld_id + 1] = 0.0;
        LightData[ld_id + 2] = 0.0;
    }

    Vec3d start(0.0, 0.0, 0.0);
    Vec3d forward(0.0, 0.0, 1.0);
    Vec3d right(1.0, 0.0, 0.0);
    Vec3d up(0.0, 1.0, 0.0);

    Vec3d ray = (forward * 1.2 + right * uv.get_x() +
                 up * uv.get_y() * (size.get_y() / size.get_x()))
                    .normalize();

    Color prev_color =
        Color(LightData[ld_id + 0], LightData[ld_id + 1], LightData[ld_id + 2]);
    Color color = measure(start, ray, 2);

    color.set_x(smoothstep(MIN(1.0, color.get_x())));
    color.set_y(smoothstep(MIN(1.0, color.get_y())));
    color.set_z(smoothstep(MIN(1.0, color.get_z())));

    color = prev_color * ((double)Iteration / (Iteration + 1)) +
            color * (1.0 / (Iteration + 1));

    LightData[ld_id + 0] = color.get_x();
    LightData[ld_id + 1] = color.get_y();
    LightData[ld_id + 2] = color.get_z();

    return sf::Color((sf::Uint8)(color.get_x() * 255.0),
                     (sf::Uint8)(color.get_y() * 255.0),
                     (sf::Uint8)(color.get_z() * 255.0));
}
