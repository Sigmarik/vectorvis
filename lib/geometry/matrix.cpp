#include "matrix.h"

// clang-format off
Mat22d::Mat22d() :
    tl_(1.0), tr_(0.0),
    bl_(0.0), br_(1.0) {}
// clang-format on

// clang-format off
Mat22d::Mat22d(double value) :
    tl_(value), tr_(0.0),
    bl_(0.0), br_(value) {}
// clang-format on

Mat22d::Mat22d(double tl, double tr, double bl, double br)
    : tl_(tl), tr_(tr), bl_(bl), br_(br) {}

// clang-format off
Mat22d::Mat22d(const Vec2d& e1, const Vec2d& e2) :
    tl_(e1.get_x()), tr_(e2.get_x()),
    bl_(e1.get_y()), br_(e2.get_y()) {}
// clang-format on

Mat22d::~Mat22d() { tl_ = tr_ = bl_ = br_ = 0.0; }

Vec2d Mat22d::operator*(const Vec2d& vector) const {
    return Vec2d(vector.get_x() * tl_ + vector.get_y() * tr_,
                 vector.get_x() * bl_ + vector.get_y() * br_);
}

Mat22d Mat22d::operator*(const Mat22d& m) const {
    return Mat22d(tl_ * m.tl_ + tr_ * m.bl_, tl_ * m.tr_ + tr_ * m.br_,
                  bl_ * m.tl_ + br_ * m.bl_, bl_ * m.tr_ + br_ * m.br_);
}

// clang-format off
Mat33d::Mat33d() :
    tl_(1.0), tm_(0.0), tr_(0.0),
    ml_(0.0), mm_(1.0), mr_(0.0),
    bl_(0.0), bm_(0.0), br_(1.0) {}
// clang-format on

// clang-format off
Mat33d::Mat33d(double value) :
    tl_(value), tm_(0.0), tr_(0.0),
    ml_(0.0), mm_(value), mr_(0.0),
    bl_(0.0), bm_(0.0), br_(value) {}
// clang-format on

// clang-format off
Mat33d::Mat33d(double tl, double tm, double tr, double ml, double mm, double mr,
               double bl, double bm, double br) :
    tl_(tl), tm_(tm), tr_(tr),
    ml_(ml), mm_(mm), mr_(mr),
    bl_(bl), bm_(bm), br_(br) {}
// clang-format on

// clang-format off
Mat33d::Mat33d(const Vec3d& e1, const Vec3d& e2, const Vec3d& e3) :
    tl_(e1.get_x()), tm_(e2.get_x()), tr_(e3.get_x()),
    ml_(e1.get_y()), mm_(e2.get_y()), mr_(e3.get_y()),
    bl_(e1.get_z()), bm_(e2.get_z()), br_(e3.get_z()) {}
// clang-format on

Mat33d::~Mat33d() {
    // clang-format off
    tl_ = 0.0; tm_ = 0.0; tr_ = 0.0;
    ml_ = 0.0; mm_ = 0.0; mr_ = 0.0;
    bl_ = 0.0; bm_ = 0.0; br_ = 0.0;
    // clang-format on
}

Vec3d Mat33d::operator*(const Vec3d& v) const {
    return Vec3d(tl_ * v.get_x() + tm_ * v.get_y() + tr_ * v.get_z(),
                 ml_ * v.get_x() + mm_ * v.get_y() + mr_ * v.get_z(),
                 bl_ * v.get_x() + bm_ * v.get_y() + br_ * v.get_z());
}

Mat33d Mat33d::operator*(const Mat33d& m) const {
    // clang-format off
    return Mat33d(tl_ * m.tl_ + tm_ * m.ml_ + tr_ * m.bl_, tl_ * m.tm_ + tm_ * m.mm_ + tr_ * m.bm_, tl_ * m.tr_ + tm_ * m.mr_ + tr_ * m.br_,
                  ml_ * m.tl_ + mm_ * m.ml_ + mr_ * m.bl_, ml_ * m.tm_ + mm_ * m.mm_ + mr_ * m.bm_, ml_ * m.tr_ + mm_ * m.mr_ + mr_ * m.br_,
                  bl_ * m.tl_ + bm_ * m.ml_ + br_ * m.bl_, bl_ * m.tm_ + bm_ * m.mm_ + br_ * m.bm_, bl_ * m.tr_ + bm_ * m.mr_ + br_ * m.br_);
    // clang-format on
}
