/**
 * @file ray_tracing_plane.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Ray tracing scene view
 * @version 0.1
 * @date 2023-09-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef RAY_TRACING_PLANE_H
#define RAY_TRACING_PLANE_H

#include <SFML/Graphics/Texture.hpp>

#include "ray_tracing.h"
#include "renderable.h"

struct RayTracingPlane : public Renderable {
    RayTracingPlane(const Vec2d& center, const Vec2d& size,
                    unsigned res_x = 512, unsigned res_y = 512);
    explicit RayTracingPlane(const RayTracingPlane& ray_tracing_plane);
    ~RayTracingPlane();

    RayTracingPlane& operator=(const RayTracingPlane& ray_tracing_plane);

    void render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                const AssetShelf& assets) override;
    void process(unsigned iterations = 1);

    const Camera& get_camera() const;
    void set_camera(const Camera& camera);

    void add_sphere(const Sphere& sphere);
    void add_plane(const Plane& plane);
    void add_light(const PointLight& light);

    PointLight& get_light(size_t id) const;
    Sphere& get_sphere(size_t id) const;
    Plane& get_plane(size_t id) const;

    void reset_progress();

    Vec2d get_center() const;
    Vec2d get_size() const;

    void set_center(const Vec2d& center);
    void set_size(const Vec2d& size);

   private:
    void iterate();

    Vec2d center_, size_;
    Scene scene_ = {};
    sf::Texture texture_;
    sf::Uint8* texture_data_;
    double* frame_data_;
    unsigned iteration_ = 0;
};

#endif
