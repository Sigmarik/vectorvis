#include "anchor.h"

#include <math.h>

Anchor::Anchor(const Vec2d& center, const Vec2d& size, const Vec2d& bind_bl,
               const Vec2d& bind_tr)
    : center_(center), size_(size), bind_bl_(bind_bl), bind_tr_(bind_tr) {
    update(current_size);
}

bool Anchor::setPosition(const Vec2d& position) {
    center_ = position;
    update(current_size);
    return true;
}

bool Anchor::setSize(const Vec2d& size) {
    size_ = size;
    update(current_size);
    return true;
}

void Anchor::onParentUpdate(const plug::LayoutBox& box) {
    current_size = box.getSize();
    update(current_size);
}

plug::LayoutBox* Anchor::clone() const { return new Anchor(*this); }

void Anchor::update(Vec2d parent_size) {
    bool split_x = abs(bind_bl_.x - bind_tr_.x) > 1e-5;
    bool split_y = abs(bind_bl_.y - bind_tr_.y) > 1e-5;

    Vec2d new_bl = parent_size / -2.0 + ANCHOR_DEFINITION_SIZE / 2.0 + bind_bl_;
    Vec2d new_tr =
        parent_size / 2.0 - (ANCHOR_DEFINITION_SIZE / 2.0 - bind_tr_);

    if (!split_x) {
        double new_x = bind_bl_.x * parent_size.x;
        new_bl.x = new_x;
        new_tr.x = new_x;
    }
    if (!split_y) {
        double new_y = bind_bl_.y * parent_size.y;
        new_bl.y = new_y;
        new_tr.y = new_y;
    }

    Vec2d new_center = (new_bl + new_tr) / 2.0;
    Vec2d new_size = (new_tr - new_bl);

    vis_size_.x = split_x ? new_size.x / (bind_tr_ - bind_bl_).x : 1.0;
    vis_size_.y = split_y ? new_size.y / (bind_tr_ - bind_bl_).y : 1.0;
    vis_center_ = new_center + center_ * vis_size_;
    vis_size_ = vis_size_ * size_;

    // vis_center_ = center_;
    // vis_size_ = size_;
}
