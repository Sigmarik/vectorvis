/**
 * @file image_view.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Editable image view
 * @version 0.1
 * @date 2023-10-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include <SFML/Graphics/Image.hpp>

#include "interactive.h"

/**
 * @brief Interactive image preview
 *
 */
struct ImageView : Interactive {
    ImageView(Vec2d center, Vec2d size, const char* file_name);
    ImageView(Vec2d center, Vec2d size, unsigned width, unsigned height);

    /**
     * @brief Set horizontal scroll position of the image view
     *
     * @param position
     */
    void scroll_x(double position);

    /**
     * @brief Set vertical scroll position of the image view
     *
     * @param position
     */
    void scroll_y(double position);

    /**
     * @brief Set image preview scale
     *
     * @param new_scale
     */
    void set_scale(double new_scale) { scale_ = new_scale; }

    /**
     * @brief Get image preview scale
     *
     * @return double
     */
    double get_scale() const { return scale_; }

    void on_event(MatrixStack<Mat33d>& stack, Interaction interaction) override;
    void render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                const AssetShelf& assets) override;

   private:
    sf::Image image_;
    Vec2d img_rect_center_ = Vec2d(0.5, 0.5);
    double scale_ = 1.0;
    bool is_active_ = false;
};

#endif
