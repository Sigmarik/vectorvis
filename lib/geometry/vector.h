/**
 * @file vector.h
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

static const double DEFAULT_VECTOR_THRESHOLD = 1e-5;

struct Vec2d {
    Vec2d();
    Vec2d(double azimuth);
    Vec2d(double x, double y);
    Vec2d(const Vec2d& vector);
    ~Vec2d();

    static Vec2d radial(double angle, double arm = 1.0);

    Vec2d& operator=(const Vec2d& vector);

    Vec2d operator+(const Vec2d& vector) const;
    Vec2d operator-(const Vec2d& vector) const;
    Vec2d operator-() const;
    Vec2d operator*(double scalar) const;
    Vec2d operator/(double scalar) const;

    Vec2d& operator+=(const Vec2d& vector);
    Vec2d& operator-=(const Vec2d& vector);
    Vec2d& operator*=(double scalar);
    Vec2d& operator/=(double scalar);

    bool operator==(Vec2d vector) const;

    Vec2d normalize(double threshold = DEFAULT_VECTOR_THRESHOLD) const;

    double dot(const Vec2d& vector) const;
    double cross(const Vec2d& vector) const;

    double length() const;
    double length2() const;

    double azimuth(double threshold = DEFAULT_VECTOR_THRESHOLD) const;
    double angle(const Vec2d& vector) const;

    Vec2d project_on_axis(const Vec2d& axis) const;
    Vec2d project_on_plane(const Vec2d& normal) const;
    Vec2d rotate(double angle) const;

    Vec2d right() const;
    Vec2d left() const;
    Vec2d back() const;

    double get_x() const;
    double get_y() const;

    void set_x(double x);
    void set_y(double y);

   private:
    double x_, y_;
};

typedef Vec2d Point;

#endif
