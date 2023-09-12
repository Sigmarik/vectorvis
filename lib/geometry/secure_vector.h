/**
 * @file vector.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Absolutely foolproof vector classes
 * @version 0.1
 * @date 2023-09-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SECURE_VECTOR_H
#define SECURE_VECTOR_H

static const double DEFAULT_SECURE_VEC_THRESHOLD = 1e-5;

struct SecureVec2d {
    SecureVec2d();
    SecureVec2d(double azimuth);
    SecureVec2d(double x, double y);
    SecureVec2d(const SecureVec2d& vector);
    ~SecureVec2d();

    static SecureVec2d radial(double angle, double arm = 1.0);

    SecureVec2d& operator=(const SecureVec2d& vector);

    SecureVec2d operator+(const SecureVec2d& vector) const;
    SecureVec2d operator-(const SecureVec2d& vector) const;
    SecureVec2d operator-() const;
    SecureVec2d operator*(double scalar) const;
    SecureVec2d operator/(double scalar) const;

    void operator+=(const SecureVec2d& vector);
    void operator-=(const SecureVec2d& vector);
    void operator*=(double scalar);
    void operator/=(double scalar);

    bool operator==(SecureVec2d vector) const;

    SecureVec2d normalize(
        double threshold = DEFAULT_SECURE_VEC_THRESHOLD) const;

    double length() const;
    double length2() const;

    double azimuth(double threshold = DEFAULT_SECURE_VEC_THRESHOLD) const;
    double angle(const SecureVec2d& vector) const;

    SecureVec2d project_on_axis(const SecureVec2d& axis) const;
    SecureVec2d project_on_plane(const SecureVec2d& normal) const;
    SecureVec2d rotate(double angle) const;

    SecureVec2d right() const;
    SecureVec2d left() const;
    SecureVec2d back() const;

    double get_x() const;
    double get_y() const;

    void set_x(double x);
    void set_y(double y);

   private:
    double x_, y_;
};

double dot(const SecureVec2d& alpha, const SecureVec2d& beta);
double cross(const SecureVec2d& alpha, const SecureVec2d& beta);

struct SecureVec3d {
    explicit SecureVec3d();
    explicit SecureVec3d(double x, double y, double z);
    explicit SecureVec3d(const SecureVec3d& vector);
    ~SecureVec3d();

    SecureVec3d& operator=(const SecureVec3d& vector);

    SecureVec3d operator+(const SecureVec3d& vector) const;
    SecureVec3d operator-(const SecureVec3d& vector) const;
    SecureVec3d operator*(double scalar) const;
    SecureVec3d operator/(double scalar) const;

    void operator+=(const SecureVec3d& vector);
    void operator-=(const SecureVec3d& vector);
    void operator*=(double scalar);
    void operator/=(double scalar);

    double length() const;
    double length2() const;

    double get_x() const;
    double get_y() const;
    double get_z() const;

    void set_x(double x);
    void set_y(double y);
    void set_z(double z);

   private:
    double x_, y_, z_;
};

double dot(const SecureVec3d& alpha, const SecureVec3d& beta);
SecureVec3d cross(const SecureVec3d& alpha, const SecureVec3d& beta);

#endif
