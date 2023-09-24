/**
 * @file vector.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Vector-like classes
 * @version 0.1
 * @date 2023-09-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef VECTOR_H
#define VECTOR_H

#pragma GCC diagnostic ignored "-Wunused-function"

#include <math.h>

static const double DEFAULT_VECTOR_THRESHOLD = 1e-5;

struct Vec2d;
struct Vec3d;

static double dot(const Vec2d& alpha, const Vec2d& beta);
static double cross(const Vec2d& alpha, const Vec2d& beta);

static double dot(const Vec3d& alpha, const Vec3d& beta);
static Vec3d cross(const Vec3d& alpha, const Vec3d& beta);

struct Vec2d {
    Vec2d() : x_(0.0), y_(0.0) {}
    explicit Vec2d(double azimuth) : x_(cos(azimuth)), y_(sin(azimuth)) {}
    Vec2d(double x, double y) : x_(x), y_(y) {}
    ~Vec2d() {
        x_ = 0.0;
        y_ = 0.0;
    }

    static Vec2d radial(double angle, double arm) { return Vec2d(angle) * arm; }

    Vec2d operator+(const Vec2d& vector) const {
        return Vec2d(x_ + vector.x_, y_ + vector.y_);
    }
    Vec2d operator-(const Vec2d& vector) const {
        return Vec2d(x_ - vector.x_, y_ - vector.y_);
    }
    Vec2d operator-() const { return Vec2d(-x_, -y_); }
    Vec2d operator*(const Vec2d& vector) const {
        return Vec2d(x_ * vector.x_, y_ * vector.y_);
    }
    Vec2d operator*(double scalar) const {
        return Vec2d(x_ * scalar, y_ * scalar);
    }
    Vec2d operator/(double scalar) const {
        return Vec2d(x_ / scalar, y_ / scalar);
    }

    void operator+=(const Vec2d& vector) {
        x_ += vector.x_;
        y_ += vector.y_;
    }
    void operator-=(const Vec2d& vector) {
        x_ -= vector.x_;
        y_ -= vector.y_;
    }
    void operator*=(double scalar) {
        x_ *= scalar;
        y_ *= scalar;
    }
    void operator/=(double scalar) {
        x_ /= scalar;
        y_ /= scalar;
    }

    bool operator==(Vec2d vector) const {
        return abs(get_x() - vector.get_x()) < 1e-7 &&
               abs(get_y() - vector.get_y()) < 1e-7;
    }

    Vec2d normalize(double threshold = DEFAULT_VECTOR_THRESHOLD) const {
        double len = length();
        if (len > -threshold && len < threshold) {
            return *this;
        }
        return *this / len;
    }

    double length() const { return sqrt(x_ * x_ + y_ * y_); }
    double length2() const { return x_ * x_ + y_ * y_; }

    double azimuth(double threshold = DEFAULT_VECTOR_THRESHOLD) const {
        if (abs(length()) < threshold) {
            return 0.0;
        }
        Vec2d normalized = normalize(threshold);
        return normalized.y_ > 0.0 ? acos(normalized.x_) : -acos(normalized.x_);
    }

    double angle(const Vec2d& vector) const {
        return vector.azimuth() - azimuth();
    }

    Vec2d project_on_axis(const Vec2d& axis) const {
        return axis * (dot(*this, axis) / axis.length2());
    }
    Vec2d project_on_plane(const Vec2d& normal) const {
        return project_on_axis(normal.right());
    }

    Vec2d rotate(double angle) const {
        double final_angle = azimuth() + angle;
        double arm = length();
        return Vec2d(arm * cos(final_angle), arm * sin(final_angle));
    }

    Vec2d right() const { return Vec2d(y_, -x_); }
    Vec2d left() const { return Vec2d(-y_, x_); }
    Vec2d back() const { return Vec2d(-x_, -y_); }

    double get_x() const { return x_; }
    double get_y() const { return y_; }

    void set_x(double x) { x_ = x; }
    void set_y(double y) { y_ = y; }

   private:
    double x_ = 0.0, y_ = 0.0;
};

static double dot(const Vec2d& alpha, const Vec2d& beta) {
    return alpha.get_x() * beta.get_x() + alpha.get_y() * beta.get_y();
}

static double cross(const Vec2d& alpha, const Vec2d& beta) {
    return alpha.get_x() * beta.get_y() - alpha.get_y() * beta.get_x();
}

struct Vec3d {
    Vec3d() : x_(0.0), y_(0.0), z_(0.0) {}
    Vec3d(double x, double y, double z) : x_(x), y_(y), z_(z) {}
    ~Vec3d() {
        x_ = 0.0;
        y_ = 0.0;
        z_ = 0.0;
    }

    Vec3d operator+(const Vec3d& vector) const {
        return Vec3d(x_ + vector.x_, y_ + vector.y_, z_ + vector.z_);
    }
    Vec3d operator-(const Vec3d& vector) const {
        return Vec3d(x_ - vector.x_, y_ - vector.y_, z_ - vector.z_);
    }
    Vec3d operator*(const Vec3d& vector) const {
        return Vec3d(x_ * vector.x_, y_ * vector.y_, z_ * vector.z_);
    }
    Vec3d operator*(double scalar) const {
        return Vec3d(x_ * scalar, y_ * scalar, z_ * scalar);
    }
    Vec3d operator/(double scalar) const {
        return Vec3d(x_ / scalar, y_ / scalar, z_ / scalar);
    }
    Vec3d operator-() const { return Vec3d(-x_, -y_, -z_); }

    void operator+=(const Vec3d& vector) {
        x_ += vector.x_;
        y_ += vector.y_;
        z_ += vector.z_;
    }
    void operator-=(const Vec3d& vector) {
        x_ -= vector.x_;
        y_ -= vector.y_;
        z_ -= vector.z_;
    }
    void operator*=(const Vec3d& vector) {
        x_ *= vector.x_;
        y_ *= vector.y_;
        z_ *= vector.z_;
    }
    void operator*=(double scalar) {
        x_ *= scalar;
        y_ *= scalar;
        z_ *= scalar;
    }
    void operator/=(double scalar) {
        x_ /= scalar;
        y_ /= scalar;
        z_ /= scalar;
    }

    double length() const { return sqrt(x_ * x_ + y_ * y_ + z_ * z_); }
    double length2() const { return x_ * x_ + y_ * y_ + z_ * z_; }

    Vec3d normalize(double threshold = DEFAULT_VECTOR_THRESHOLD) const {
        double len = length();
        if (len > -threshold && len < threshold) {
            return *this;
        }
        return *this / len;
    }

    double get_x() const { return x_; }
    double get_y() const { return y_; }
    double get_z() const { return z_; }

    void set_x(double x) { x_ = x; }
    void set_y(double y) { y_ = y; }
    void set_z(double z) { z_ = z; }

   private:
    double x_, y_, z_;
};

static double dot(const Vec3d& alpha, const Vec3d& beta) {
    return alpha.get_x() * beta.get_x() + alpha.get_y() * beta.get_y() +
           alpha.get_z() * beta.get_z();
}

static Vec3d cross(const Vec3d& alpha, const Vec3d& beta) {
    return Vec3d(alpha.get_y() * beta.get_z() - alpha.get_z() * beta.get_y(),
                 alpha.get_z() * beta.get_x() - alpha.get_x() * beta.get_z(),
                 alpha.get_x() * beta.get_y() - alpha.get_y() * beta.get_x());
}

typedef Vec2d Point;

#endif
