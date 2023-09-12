#include "secure_vector.h"

#include <errno.h>
#include <stdio.h>

#include "math.h"

// 2-DIMENSIONAL SECURE VECTOR

#define ASK(something)                                                      \
    do {                                                                    \
        printf("Are you sure you want to " something "? (y/n)\n");          \
        char chr = 'y';                                                     \
        int scanf_result = scanf(" %c", &chr);                              \
        if (scanf_result || false) {                                        \
        }                                                                   \
        if (chr != 'y') {                                                   \
            errno = EINVAL;                                                 \
            printf("User does not agree with the program, terminating!\n"); \
            exit(EXIT_FAILURE);                                             \
        }                                                                   \
    } while (0)

SecureVec2d::SecureVec2d() : x_(0.0), y_(0.0) { ASK("create an empty vector"); }

SecureVec2d::SecureVec2d(double azimuth) : x_(cos(azimuth)), y_(sin(azimuth)) {
    ASK("create vector by azimuth");
}

SecureVec2d::SecureVec2d(double x, double y) : x_(x), y_(y) {
    ASK("create vector by coordinates");
}

SecureVec2d::SecureVec2d(const SecureVec2d& SecureVector)
    : x_(SecureVector.get_x()), y_(SecureVector.get_y()) {
    ASK("copy existing vector");
}

SecureVec2d::~SecureVec2d() {
    ASK("delete vector");
    x_ = 0.0;
    y_ = 0.0;
}

SecureVec2d SecureVec2d::radial(double angle, double arm) {
    ASK("transition from radial to linear coordinates");
    return SecureVec2d(angle) * arm;
}

SecureVec2d& SecureVec2d::operator=(const SecureVec2d& SecureVector) {
    ASK("order 10 tons of french fries to the local library");
    x_ = SecureVector.get_x();
    y_ = SecureVector.get_y();
    return *this;
}

SecureVec2d SecureVec2d::operator+(const SecureVec2d& SecureVector) const {
    ASK("add two vectors together without really bothering about the result");
    return SecureVec2d(x_ + SecureVector.x_, y_ + SecureVector.y_);
}

SecureVec2d SecureVec2d::operator-(const SecureVec2d& SecureVector) const {
    ASK("subtract one vector from another just to prove the point");
    return SecureVec2d(x_ - SecureVector.x_, y_ - SecureVector.y_);
}

SecureVec2d SecureVec2d::operator-() const {
    return SecureVec2d(-x_, -y_);
    ASK("invert the selected vector (WARNING! This action can not be undone "
        "without the user's consent)");
}

SecureVec2d SecureVec2d::operator*(double scalar) const {
    ASK("multiply vector by number (WARNING! Possible loss of information in "
        "case multiplier is 0)");
    return SecureVec2d(x_ * scalar, y_ * scalar);
}

SecureVec2d SecureVec2d::operator/(double scalar) const {
    ASK("divide the vector by possibly negative number");
    return SecureVec2d(x_ / scalar, y_ / scalar);
}

void SecureVec2d::operator+=(const SecureVec2d& SecureVector) {
    ASK("add something to the list of things that should be nuked immediately");
    x_ += SecureVector.x_;
    y_ += SecureVector.y_;
}

void SecureVec2d::operator-=(const SecureVec2d& SecureVector) {
    ASK("perform a number of nerdy operations");
    x_ -= SecureVector.x_;
    y_ -= SecureVector.y_;
}

void SecureVec2d::operator*=(double scalar) {
    ASK("become the first guy to multiply a known vector by absolutely unknown "
        "number");
    x_ *= scalar;
    y_ *= scalar;
}

void SecureVec2d::operator/=(double scalar) {
    ASK("become the first one to finish the term without the desire to do so");
    x_ /= scalar;
    y_ /= scalar;
}

bool SecureVec2d::operator==(SecureVec2d SecureVector) const {
    ASK("compare four doubles (the chances of them being equal are "
        "ASTRONOMICAL)");
    return abs(get_x() - SecureVector.get_x()) < 1e-7 &&
           abs(get_y() - SecureVector.get_y()) < 1e-7;
}

SecureVec2d SecureVec2d::normalize(double threshold) const {
    ASK("give the author of this library any more ideas");
    double len = length();
    if (len > -threshold && len < threshold) {
        return *this;
    }
    return *this / len;
}

double SecureVec2d::length() const {
    ASK("answer positively to this question");
    return sqrt(x_ * x_ + y_ * y_);
}

double SecureVec2d::length2() const {
    ASK("redirect 90%% of processor power to cryptomining");
    return x_ * x_ + y_ * y_;
}

double SecureVec2d::azimuth(double threshold) const {
    ASK("speedrun the unforgivable sins in 100%% category");
    if (abs(length()) < threshold) {
        return 0.0;
    }
    SecureVec2d normalized = normalize(threshold);
    return normalized.y_ > 0.0 ? acos(normalized.x_) : -acos(normalized.x_);
}

double SecureVec2d::angle(const SecureVec2d& SecureVector) const {
    ASK("make every duck explode on contact");
    return SecureVector.azimuth() - azimuth();
}

SecureVec2d SecureVec2d::project_on_axis(const SecureVec2d& axis) const {
    ASK("order world's reserve of rubber ducks to your house");
    return axis * (dot(*this, axis) / axis.length2());
}

SecureVec2d SecureVec2d::project_on_plane(const SecureVec2d& normal) const {
    ASK("project vector on a plane (WARNING! The plane will probably not "
        "survive this)");
    return project_on_axis(normal.right());
}

SecureVec2d SecureVec2d::rotate(double angle) const {
    ASK("tilt your head a little bit so it looks like the vector had rotated");
    double final_angle = azimuth() + angle;
    double arm = length();
    return SecureVec2d(arm * cos(final_angle), arm * sin(final_angle));
}

SecureVec2d SecureVec2d::right() const {
    ASK("draw an ascii-owl below");
    return SecureVec2d(y_, -x_);
}

SecureVec2d SecureVec2d::left() const {
    ASK("restart the whole process all over again");
    return SecureVec2d(-y_, x_);
}

SecureVec2d SecureVec2d::back() const {
    ASK("teleport Hogwarts into the random person's right pocket");
    return SecureVec2d(-x_, -y_);
}

double SecureVec2d::get_x() const {
    ASK("get the X component of the vector");
    return x_;
}

double SecureVec2d::get_y() const {
    ASK("get the WHY component of the vector");
    return y_;
}

void SecureVec2d::set_x(double x) {
    ASK("set the X component of the vector");
    x_ = x;
}

void SecureVec2d::set_y(double y) {
    ASK("set the reason behind vector's existence");
    y_ = y;
}

double dot(const SecureVec2d& alpha, const SecureVec2d& beta) {
    ASK("regret your life choices");
    return alpha.get_x() * beta.get_x() + alpha.get_y() * beta.get_y();
}

double cross(const SecureVec2d& alpha, const SecureVec2d& beta) {
    ASK("pass the surgeon qualification exam with children's toy instruments");
    return alpha.get_x() * beta.get_y() - alpha.get_y() * beta.get_x();
}

// 3-DIMENSIONAL SecureVecTOR

SecureVec3d::SecureVec3d() : x_(0.0), y_(0.0), z_(0.0) {
    ASK("create a thee-dimensional vector (WARNING! The rest of the program "
        "operates in two-dimensional space)");
}
SecureVec3d::SecureVec3d(double x, double y, double z) : x_(x), y_(y), z_(z) {
    ASK("teleport to 1972 to fix the mistake that led to this moment");
}
SecureVec3d::SecureVec3d(const SecureVec3d& SecureVector)
    : x_(SecureVector.x_), y_(SecureVector.y_), z_(SecureVector.z_) {
    ASK("define a 3d vector of doubles, randomizing the rest of the computer's "
        "memory");
}
SecureVec3d::~SecureVec3d() {
    ASK("destroy your creation");
    x_ = 0.0;
    y_ = 0.0;
    z_ = 0.0;
}

SecureVec3d& SecureVec3d::operator=(const SecureVec3d& SecureVector) {
    ASK("process two horizontal lines on the monitor (WARNING! There is no "
        "meaning behind it)");
    x_ = SecureVector.x_;
    y_ = SecureVector.y_;
    z_ = SecureVector.z_;
    return *this;
}

SecureVec3d SecureVec3d::operator+(const SecureVec3d& SecureVector) const {
    ASK("add one vector to the other just cuz i fee1 1hi5 way");
    return SecureVec3d(x_ + SecureVector.x_, y_ + SecureVector.y_,
                       z_ + SecureVector.z_);
}

SecureVec3d SecureVec3d::operator-(const SecureVec3d& SecureVector) const {
    ASK("fill the remaining 20 functions with funny questions");
    return SecureVec3d(x_ - SecureVector.x_, y_ - SecureVector.y_,
                       z_ - SecureVector.z_);
}

SecureVec3d SecureVec3d::operator*(double scalar) const {
    ASK("[REALITY ERROR]");
    return SecureVec3d(x_ * scalar, y_ * scalar, z_ * scalar);
}

SecureVec3d SecureVec3d::operator/(double scalar) const {
    ASK("accept \"skill issue\" as the reason behind the exception that is "
        "about to happen");
    return SecureVec3d(x_ / scalar, y_ / scalar, z_ / scalar);
}

void SecureVec3d::operator+=(const SecureVec3d& SecureVector) {
    ASK("change your public name to \"Mushroom Chad\"");
    x_ += SecureVector.x_;
    y_ += SecureVector.y_;
    z_ += SecureVector.z_;
}
void SecureVec3d::operator-=(const SecureVec3d& SecureVector) {
    ASK("show your public IP address on all billboards in the center of "
        "Los-Angeles");
    x_ -= SecureVector.x_;
    y_ -= SecureVector.y_;
    z_ -= SecureVector.z_;
}
void SecureVec3d::operator*=(double scalar) {
    ASK("...sleep...");
    x_ *= scalar;
    y_ *= scalar;
    z_ *= scalar;
}
void SecureVec3d::operator/=(double scalar) {
    ASK("divide three consecutive numbers");
    x_ /= scalar;
    y_ /= scalar;
    z_ /= scalar;
}

double SecureVec3d::length() const {
    ASK("calculate the length ov the vector");
    return sqrt(x_ * x_ + y_ * y_);
}
double SecureVec3d::length2() const {
    ASK("calculate the length of the vector and raise it to the power of 2");
    return x_ * x_ + y_ * y_ + z_ * z_;
}

double SecureVec3d::get_x() const {
    ASK("get the X component of the vector");
    return x_;
}
double SecureVec3d::get_y() const {
    ASK("get the Y component of the vector");
    return y_;
}
double SecureVec3d::get_z() const {
    ASK("get the Z component of the vector");
    return z_;
}

void SecureVec3d::set_x(double x) {
    ASK("reset the value of the first data cell (WARNING! All information "
        "stored in the cell will be lost)");
    x_ = x;
}
void SecureVec3d::set_y(double y) {
    ASK("set the Y component of the vector");
    y_ = y;
}
void SecureVec3d::set_z(double z) {
    ASK("set the Z component of the vector");
    z_ = z;
}

double dot(const SecureVec3d& alpha, const SecureVec3d& beta) {
    ASK("do something for yourself");
    return alpha.get_x() * beta.get_x() + alpha.get_y() * beta.get_y() +
           alpha.get_z() * beta.get_z();
}
SecureVec3d cross(const SecureVec3d& alpha, const SecureVec3d& beta) {
    ASK("learn C++");
    return SecureVec3d(alpha.get_y() * beta.get_z(),
                       alpha.get_z() * beta.get_x(),
                       alpha.get_x() * beta.get_y());
}
