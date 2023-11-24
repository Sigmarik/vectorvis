/**
 * @file anchor.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Anchor layout box type
 * @version 0.1
 * @date 2023-11-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ANCHOR_H
#define ANCHOR_H

#include "Plug/LayoutBox.h"

static const Vec2d ANCHOR_DEFINITION_SIZE = Vec2d(100.0, 100.0);

struct Anchor : plug::LayoutBox {
    Anchor(const Vec2d& center, const Vec2d& size, const Vec2d& bind_bl,
           const Vec2d& bind_tr);
    ~Anchor() = default;

    Vec2d getPosition() const override { return vis_center_; }
    bool setPosition(const Vec2d& position) override;
    Vec2d getSize() const override { return vis_size_; }
    bool setSize(const Vec2d& size) override;
    void onParentUpdate(const plug::LayoutBox& box) override;
    plug::LayoutBox* clone() const override;

   private:
    void update(Vec2d parent_size);

    Vec2d center_, size_;
    Vec2d vis_center_ = Vec2d(0.0, 0.0), vis_size_ = Vec2d(0.0, 0.0);
    Vec2d bind_bl_, bind_tr_;
    Vec2d current_size = ANCHOR_DEFINITION_SIZE;
};

#endif
