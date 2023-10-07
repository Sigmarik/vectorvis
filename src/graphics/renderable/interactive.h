/**
 * @file interactive.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Interactive graphical element
 * @version 0.1
 * @date 2023-09-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>

#include "renderable.h"
#include "sf_cheatsheet.hpp"

/**
 * @brief Program timer
 *
 */
struct WorldTimer final {
    static unsigned long long get();

   private:
    WorldTimer();
    WorldTimer& operator=(const WorldTimer& timer);
    sf::Clock clock_;
};

/**
 * @brief Element used to describe object behaviour with parents of different
 * sizes
 *
 */
struct Anchor {
    Anchor() : center_(0.0, 0.0), size_(0.0, 0.0) {}
    Anchor(Vec2d center, Vec2d size) : center_(center), size_(size) {}
    Anchor(const Anchor& anchor) = default;

    Anchor& operator=(const Anchor& anchor) = default;

    /**
     * @brief Get anchor box center point
     *
     * @return Vec2d
     */
    Vec2d get_center() { return center_; }

    /**
     * @brief Get anchor box dimensions
     *
     * @return Vec2d
     */
    Vec2d get_size() { return size_; }

   private:
    Vec2d center_;
    Vec2d size_;
};

/**
 * @brief User interaction descriptor
 *
 */
struct Interaction {
    sf::Event* event = NULL;
    sf::Window* window = NULL;
    bool obstructed = false;
};

/**
 * @brief Box corner index
 *
 */
enum Corner {
    TOP_LEFT = 0,
    TOP_RIGHT = 1,
    BOTTOM_LEFT = 2,
    BOTTOM_RIGHT = 3,
};

/**
 * @brief Rectangular object that is able to process user events
 *
 */
struct Interactive : public Renderable {
    Interactive(const Vec2d& center, const Vec2d& size);
    virtual ~Interactive() = default;

    /**
     * @brief Handle user event
     *
     * @param stack
     * @param interaction
     */
    virtual void on_event(MatrixStack<Mat33d>& stack, Interaction interaction);

    /**
     * @brief Get object center point
     *
     * @return Vec2d
     */
    Vec2d get_center() const { return center_; }

    /**
     * @brief Get object dimensions
     *
     * @return Vec2d
     */
    Vec2d get_size() const { return size_; }

    /**
     * @brief Get visual size of the object (size with applied anchor
     * transforms)
     *
     * @return Vec2d
     */
    Vec2d get_vis_size() const { return vis_size_; }

    /**
     * @brief Get visual center of the object (with applied object transforms)
     *
     * @return Vec2d
     */
    Vec2d get_vis_center() const { return vis_center_; }

    /**
     * @brief Set object center point
     *
     * @param new_center
     */
    void set_center(const Vec2d& new_center) {
        center_ = new_center;
        vis_center_ = center_;
    }

    /**
     * @brief Set object size
     *
     * @param new_size
     */
    void set_size(const Vec2d& new_size) {
        size_ = new_size;
        vis_size_ = size_;
    }

    /**
     * @brief Set object anchor
     *
     * @param anchor
     */
    void set_anchor(const Anchor& anchor) { anchor_ = anchor; }

    /**
     * @brief Get object anchor
     *
     * @return Anchor
     */
    Anchor get_anchor() const { return anchor_; }

    /**
     * @brief Update visual center and size of the object to the new parent size
     *
     * @param parent_size
     */
    virtual void apply_anchor(const Vec2d& parent_size);

    /**
     * @brief Check if object covers given position of the screen
     *
     * @param stack object transform
     * @param screen_pos position to check
     * @return true if the object box covers the specified position
     * @return false otherwise
     */
    bool is_under(const MatrixStack<Mat33d>& stack, const Vec2d& screen_pos) {
        // clang-format off
        Vec2d tl = get_corner(TOP_LEFT,     stack);
        Vec2d tr = get_corner(TOP_RIGHT,    stack);
        Vec2d br = get_corner(BOTTOM_RIGHT, stack);
        Vec2d bl = get_corner(BOTTOM_LEFT,  stack);

        bool top_check      = cross(tl - tr, screen_pos - tr) <= 0.0;
        bool right_check    = cross(tr - br, screen_pos - br) <= 0.0;
        bool bottom_check   = cross(br - bl, screen_pos - bl) <= 0.0;
        bool left_check     = cross(bl - tl, screen_pos - tl) <= 0.0;
        // clang-format on

        return top_check && right_check && bottom_check && left_check;
    }

    /**
     * @brief Tick the element
     *
     */
    virtual void tick() {}

    /**
     * @brief Set custom transform for the object
     *
     * @param matrix
     */
    void set_custom_transform(Mat33d matrix) { custom_transform_ = matrix; }

   protected:
    /**
     * @brief Get position of the corner of the object
     *
     * @param corner corner id
     * @param stack object transform
     * @return Vec2d
     */
    Vec2d get_corner(Corner corner, const MatrixStack<Mat33d>& stack) {
        Vec2d direction =
            Vec2d((corner & 1) ? 0.5 : -0.5, (corner & 2) ? -0.5 : 0.5);
        Vec3d transformed =
            stack.top() * get_matrix() * extrude(vis_size_ * direction);
        return Vec2d(transformed.get_x(), transformed.get_y()) /
               transformed.get_z();
    }

    /**
     * @brief Get object matrix
     *
     * @return Mat33d
     */
    Mat33d get_matrix() {
        return Mat33d(1.0, 0.0, vis_center_.get_x(), 0.0, 1.0,
                      vis_center_.get_y(), 0.0, 0.0, 1.0) *
               custom_transform_;
    }

    Vec2d center_ = Vec2d(), size_ = Vec2d(1.0, 1.0);
    Vec2d vis_center_ = Vec2d(), vis_size_ = Vec2d(1.0, 1.0);
    Vec2d mouse_position_ = Vec2d(0.0, 0.0);

    Mat33d custom_transform_ = Mat33d(1.0);

    Anchor anchor_ = Anchor();
};

#endif
