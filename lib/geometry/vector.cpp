#include "vector.h"

#include "math.h"

Vec2d::Vec2d() : x_(0.0), y_(0.0) {}

Vec2d::Vec2d(double azimuth) : x_(cos(azimuth)), y_(sin(azimuth)) {}

Vec2d::Vec2d(double x, double y) : x_(x), y_(y) {}

Vec2d::Vec2d(const Vec2d& vector) : x_(vector.get_x()), y_(vector.get_y()) {}

Vec2d::~Vec2d() {
    x_ = 0.0;
    y_ = 0.0;
}

Vec2d Vec2d::radial(double angle, double arm) { return Vec2d(angle) * arm; }

Vec2d& Vec2d::operator=(const Vec2d& vector) {
    x_ = vector.get_x();
    y_ = vector.get_y();
    return *this;
}

Vec2d Vec2d::operator+(const Vec2d& vector) const {
    return Vec2d(x_ + vector.x_, y_ + vector.y_);
}

Vec2d Vec2d::operator-(const Vec2d& vector) const {
    return Vec2d(x_ - vector.x_, y_ - vector.y_);
}

Vec2d Vec2d::operator-() const { return Vec2d(-x_, -y_); }

Vec2d Vec2d::operator*(double scalar) const {
    return Vec2d(x_ * scalar, y_ * scalar);
}

Vec2d Vec2d::operator/(double scalar) const {
    return Vec2d(x_ / scalar, y_ / scalar);
}

Vec2d& Vec2d::operator+=(const Vec2d& vector) {
    x_ += vector.x_;
    y_ += vector.y_;
    return *this;
}

Vec2d& Vec2d::operator-=(const Vec2d& vector) {
    x_ -= vector.x_;
    y_ -= vector.y_;
    return *this;
}

Vec2d& Vec2d::operator*=(double scalar) {
    x_ *= scalar;
    y_ *= scalar;
    return *this;
}

Vec2d& Vec2d::operator/=(double scalar) {
    x_ /= scalar;
    y_ /= scalar;
    return *this;
}

bool Vec2d::operator==(Vec2d vector) const {
    return abs(get_x() - vector.get_x()) < 1e-7 &&
           abs(get_y() - vector.get_y()) < 1e-7;
}

Vec2d Vec2d::normalize(double threshold) const {
    double len = length();
    if (len > -threshold && len < threshold) {
        return *this;
    }
    return *this / len;
}

double Vec2d::dot(const Vec2d& vector) const {
    return x_ * vector.x_ + y_ * vector.y_;
}

double Vec2d::cross(const Vec2d& vector) const {
    return x_ * vector.y_ - y_ * vector.x_;
}

double Vec2d::length() const { return sqrt(x_ * x_ + y_ * y_); }

double Vec2d::length2() const { return x_ * x_ + y_ * y_; }

double Vec2d::azimuth(double threshold) const {
    if (abs(length()) < threshold) {
        return 0.0;
    }
    Vec2d normalized = normalize(threshold);
    return normalized.y_ > 0.0 ? acos(normalized.x_) : -acos(normalized.x_);
}

double Vec2d::angle(const Vec2d& vector) const {
    return vector.azimuth() - azimuth();
}

Vec2d Vec2d::project_on_axis(const Vec2d& axis) const {
    return axis * (dot(axis) / axis.length2());
}

Vec2d Vec2d::project_on_plane(const Vec2d& normal) const {
    return project_on_axis(normal.right());
}

Vec2d Vec2d::rotate(double angle) const {
    double final_angle = azimuth() + angle;
    double arm = length();
    return Vec2d(arm * cos(final_angle), arm * sin(final_angle));
}

Vec2d Vec2d::right() const { return Vec2d(y_, -x_); }

Vec2d Vec2d::left() const { return Vec2d(-y_, x_); }

Vec2d Vec2d::back() const { return Vec2d(-x_, -y_); }

double Vec2d::get_x() const { return x_; }

double Vec2d::get_y() const { return y_; }

void Vec2d::set_x(double x) { x_ = x; }

void Vec2d::set_y(double y) { y_ = y; }
