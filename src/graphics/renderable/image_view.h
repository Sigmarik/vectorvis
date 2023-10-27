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

#include <SFML/Graphics/RenderTexture.hpp>

#include "interactive.h"

struct ToolPalette;
struct FilterPalette;

struct Mask {
    Mask(size_t width, size_t height);
    ~Mask();
    explicit Mask(const Mask& mask);
    Mask& operator=(const Mask& mask);

    bool get_at(size_t pos_x, size_t pos_y) const;
    void set_at(size_t pos_x, size_t pos_y, bool value);

    size_t get_width() const { return width_; }
    size_t get_height() const { return height_; }

    void fill(bool value);
    void invert();

   private:
    bool* data_;
    size_t width_ = 0, height_ = 0;
};

/**
 * @brief Interactive image preview
 *
 */
struct ImageView : Interactive {
    ImageView(ToolPalette* tools, FilterPalette* filters, Vec2d center,
              Vec2d size, const char* file_name);
    ImageView(ToolPalette* tools, FilterPalette* filters, Vec2d center,
              Vec2d size, unsigned width, unsigned height);
    ImageView(const ImageView& view) = default;
    ~ImageView() = default;

    ImageView& operator=(const ImageView& view) = default;
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

    Vec2d to_scene_coords(const Vec2d& pix_coord) const;

    sf::RenderTexture& get_texture() { return texture_; }

    void on_event(MatrixStack<Mat33d>& stack, Interaction interaction) override;
    void render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                const AssetShelf& assets) override;

    ToolPalette* get_pallet() const { return tools_; }

    void update_image();
    void update_from_image();
    sf::Image& get_image() { return image_; }

   private:
    void upd_tool(const Vec2d& img_pos, const sf::Event& event);

    sf::RenderTexture texture_;
    Vec2d img_rect_center_ = Vec2d(0.5, 0.5);
    double scale_ = 1.0;
    bool is_active_ = false;
    bool img_update_ = false;
    sf::Image image_ = sf::Image();

    ToolPalette* tools_;
    FilterPalette* filters_;

    Mask selection_;
};

#endif
