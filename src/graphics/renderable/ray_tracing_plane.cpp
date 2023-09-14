#include "ray_tracing_plane.h"

#include <SFML/Graphics/VertexArray.hpp>
#include <cstring>

#include "sf_cheatsheet.hpp"

RayTracingPlane::RayTracingPlane(const Vec2d& center, const Vec2d& size,
                                 unsigned res_x, unsigned res_y)
    : center_(center),
      size_(size),
      texture_(),
      texture_data_(
          (sf::Uint8*)calloc(res_x * res_y * 4, sizeof(*texture_data_))),
      frame_data_((double*)calloc(res_x * res_y * 4, sizeof(*frame_data_))) {
    texture_.create(res_x, res_y);
}

RayTracingPlane::RayTracingPlane(const RayTracingPlane& ray_tracing_plane)
    : center_(ray_tracing_plane.center_),
      size_(ray_tracing_plane.size_),
      texture_(),
      texture_data_(
          (sf::Uint8*)calloc(ray_tracing_plane.texture_.getSize().x *
                                 ray_tracing_plane.texture_.getSize().y * 4,
                             sizeof(*texture_data_))),
      frame_data_((double*)calloc(ray_tracing_plane.texture_.getSize().x *
                                      ray_tracing_plane.texture_.getSize().y *
                                      4,
                                  sizeof(*frame_data_))) {
    memcpy(texture_data_, ray_tracing_plane.texture_data_,
           ray_tracing_plane.texture_.getSize().x *
               ray_tracing_plane.texture_.getSize().y * 4 *
               sizeof(*texture_data_));
    memcpy(frame_data_, ray_tracing_plane.frame_data_,
           ray_tracing_plane.texture_.getSize().x *
               ray_tracing_plane.texture_.getSize().y * 4 *
               sizeof(*frame_data_));
    texture_.create(ray_tracing_plane.texture_.getSize().x,
                    ray_tracing_plane.texture_.getSize().y);
}

RayTracingPlane::~RayTracingPlane() {
    free(texture_data_);
    free(frame_data_);
}

RayTracingPlane& RayTracingPlane::operator=(
    const RayTracingPlane& ray_tracing_plane) {
    center_ = ray_tracing_plane.center_;
    size_ = ray_tracing_plane.size_;
    texture_ = sf::Texture();
    iteration_ = 0;
    texture_data_ =
        (sf::Uint8*)calloc(ray_tracing_plane.texture_.getSize().x *
                               ray_tracing_plane.texture_.getSize().y * 4,
                           sizeof(*texture_data_));
    frame_data_ =
        (double*)calloc(ray_tracing_plane.texture_.getSize().x *
                            ray_tracing_plane.texture_.getSize().y * 4,
                        sizeof(*frame_data_));
    memcpy(texture_data_, ray_tracing_plane.texture_data_,
           ray_tracing_plane.texture_.getSize().x *
               ray_tracing_plane.texture_.getSize().y * 4 *
               sizeof(*texture_data_));
    memcpy(frame_data_, ray_tracing_plane.frame_data_,
           ray_tracing_plane.texture_.getSize().x *
               ray_tracing_plane.texture_.getSize().y * 4 *
               sizeof(*frame_data_));
    texture_.create(ray_tracing_plane.texture_.getSize().x,
                    ray_tracing_plane.texture_.getSize().y);
    return *this;
}

void RayTracingPlane::render(MatrixStack<Mat33d>& stack,
                             sf::RenderTarget& target,
                             const AssetShelf& assets) {
    static sf::Vertex vertices[4];

    texture_.setSmooth(true);

    Vec2d half_size = size_ / 2.0;
    Vec2d half_size_inv = size_ / 2.0;
    half_size_inv.set_x(-half_size_inv.get_x());

    vertices[0] = to_sf(stack.top() * extrude(center_ + half_size_inv));
    vertices[1] = to_sf(stack.top() * extrude(center_ + half_size));
    vertices[2] = to_sf(stack.top() * extrude(center_ - half_size_inv));
    vertices[3] = to_sf(stack.top() * extrude(center_ - half_size));

    vertices[0].texCoords = sf::Vector2f(0.0, 0.0);
    vertices[1].texCoords = sf::Vector2f((float)texture_.getSize().x, 0.0);
    vertices[2].texCoords =
        sf::Vector2f((float)texture_.getSize().x, (float)texture_.getSize().y);
    vertices[3].texCoords = sf::Vector2f(0.0, (float)texture_.getSize().y);

    target.draw(vertices, sizeof(vertices) / sizeof(*vertices),
                sf::PrimitiveType::Quads, sf::RenderStates(&texture_));
}

void RayTracingPlane::process(unsigned iterations) {
    for (unsigned iter_id = 0; iter_id < iterations; ++iter_id) {
        iterate();
    }
}

const Camera& RayTracingPlane::get_camera() const { return scene_.camera; }

void RayTracingPlane::set_camera(const Camera& camera) {
    scene_.camera = camera;
}

void RayTracingPlane::add_sphere(const Sphere& sphere) {
    scene_.spheres.push(sphere);
}

void RayTracingPlane::add_plane(const Plane& plane) {
    scene_.planes.push(plane);
}

void RayTracingPlane::add_light(const PointLight& light) {
    scene_.lights.push(light);
}

PointLight& RayTracingPlane::get_light(size_t id) const {
    return scene_.lights[id];
}

Sphere& RayTracingPlane::get_sphere(size_t id) const {
    return scene_.spheres[id];
}

Plane& RayTracingPlane::get_plane(size_t id) const { return scene_.planes[id]; }

void RayTracingPlane::reset_progress() { iteration_ = 0; }

Vec2d RayTracingPlane::get_center() const { return center_; }

Vec2d RayTracingPlane::get_size() const { return size_; }

void RayTracingPlane::set_center(const Vec2d& center) { center_ = center; }

void RayTracingPlane::set_size(const Vec2d& size) { size_ = size; }

void RayTracingPlane::iterate() {
    Vec2d uv;
    for (unsigned x_id = 0; x_id < texture_.getSize().x; ++x_id) {
        for (unsigned y_id = 0; y_id < texture_.getSize().y; ++y_id) {
            uv.set_x(-1.0 + 2.0 * x_id / texture_.getSize().x);
            uv.set_y(1.0 - 2.0 * y_id / texture_.getSize().y);

            Color color = scene_.measure(scene_.camera.position,
                                         scene_.camera.forward +
                                             scene_.camera.right * uv.get_x() +
                                             scene_.camera.up * uv.get_y() *
                                                 size_.get_y() / size_.get_x(),
                                         2);

            unsigned id = (y_id * texture_.getSize().y + x_id) * 4;

            color.set_x(smoothstep(color.get_x()));
            color.set_y(smoothstep(color.get_y()));
            color.set_z(smoothstep(color.get_z()));

            Color prev_color = Color(frame_data_[id + 0], frame_data_[id + 1],
                                     frame_data_[id + 2]);

            color = prev_color * ((double)iteration_ / (iteration_ + 1)) +
                    color * (1.0 / (iteration_ + 1));

            frame_data_[id + 0] = color.get_x();
            frame_data_[id + 1] = color.get_y();
            frame_data_[id + 2] = color.get_z();

            texture_data_[id + 0] = (sf::Uint8)(255.0 * color.get_x());
            texture_data_[id + 1] = (sf::Uint8)(255.0 * color.get_y());
            texture_data_[id + 2] = (sf::Uint8)(255.0 * color.get_z());
            texture_data_[id + 3] = 255;
        }
    }
    texture_.update(texture_data_);

    ++iteration_;
}
