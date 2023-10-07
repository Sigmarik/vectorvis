#include "image_view.h"

#include <SFML/Graphics/VertexArray.hpp>

#include "sf_cheatsheet.hpp"
#include "src/editor_tools/tools.h"

ImageView::ImageView(Vec2d center, Vec2d size, const char* file_name)
    : Interactive(center, size), image_() {
    image_.loadFromFile(file_name);
}

ImageView::ImageView(Vec2d center, Vec2d size, unsigned width, unsigned height)
    : Interactive(center, size), image_() {
    image_.create(width, height, sf::Color(255, 255, 255));
}

static const double PIXEL_SIZE = 0.02;

void ImageView::scroll_x(double position) {
    double low = size_.get_x() * scale_ * PIXEL_SIZE / 2.0;
    double high = 1.0 - size_.get_x() * scale_ * PIXEL_SIZE / 2.0;
    if (low > high) {
        img_rect_center_.set_x(0.0);
    } else {
        img_rect_center_.set_x(high * position + low * (1.0 - position));
    }
}

void ImageView::scroll_y(double position) {
    double low = size_.get_y() * scale_ * PIXEL_SIZE / 2.0;
    double high = 1.0 - size_.get_y() * scale_ * PIXEL_SIZE / 2.0;
    if (low > high) {
        img_rect_center_.set_y(0.0);
    } else {
        img_rect_center_.set_y(high * position + low * (1.0 - position));
    }
}

void ImageView::on_event(MatrixStack<Mat33d>& stack, Interaction interaction) {
    sf::Vector2i mouse_pure = sf::Mouse::getPosition(*interaction.window);
    Vec2d mouse_pos =
        Vec2d((double)mouse_pure.x / interaction.window->getSize().x,
              (double)mouse_pure.y / interaction.window->getSize().y);

    if (interaction.event->type == sf::Event::MouseButtonReleased &&
        interaction.event->mouseButton.button == sf::Mouse::Button::Left) {
        is_active_ = false;
    }

    if (interaction.obstructed || !is_under(stack, mouse_pos)) {
        return;
    }

    if (interaction.event->type == sf::Event::MouseButtonPressed &&
        interaction.event->mouseButton.button == sf::Mouse::Button::Left) {
        is_active_ = true;
    }

    if (is_active_) {
        Vec3d mouse_rel = stack.top().inverse() *
                          Vec3d(mouse_pos.get_x(), mouse_pos.get_y(), 1.0);

        mouse_rel /= mouse_rel.get_z();

        Vec2d img_pos =
            img_rect_center_ + (Vec2d(mouse_rel.get_x(), -mouse_rel.get_y())) *
                                   (scale_ * PIXEL_SIZE);

        img_pos = img_pos * Vec2d(image_.getSize().x, image_.getSize().y);

        static EraserTool brush(5, 1.0);
        brush.use(image_, img_pos);
    }
}

void ImageView::render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                       const AssetShelf& assets) {
    static sf::VertexArray shape(sf::PrimitiveType::Quads, 4);
    static sf::Texture texture;

    texture.loadFromImage(image_);

    Vec3d center = extrude(vis_center_);
    Vec3d size = extrude(vis_size_);

    // clang-format off
    shape[0].position = to_Vector2f(get_corner(TOP_LEFT,     stack));
    shape[1].position = to_Vector2f(get_corner(TOP_RIGHT,    stack));
    shape[2].position = to_Vector2f(get_corner(BOTTOM_RIGHT, stack));
    shape[3].position = to_Vector2f(get_corner(BOTTOM_LEFT,  stack));
    // clang-format on

    Vec2d img_size(image_.getSize().x, image_.getSize().y);

    shape[0].texCoords = to_Vector2f(
        (img_rect_center_ + size_ * scale_ * PIXEL_SIZE * Vec2d(-0.5, -0.5)) *
        img_size);  // sf::Vector2f(0.0, 0.0);
    shape[1].texCoords = to_Vector2f(
        (img_rect_center_ + size_ * scale_ * PIXEL_SIZE * Vec2d(0.5, -0.5)) *
        img_size);  // sf::Vector2f(1.0, 0.0);
    shape[2].texCoords = to_Vector2f(
        (img_rect_center_ + size_ * scale_ * PIXEL_SIZE * Vec2d(0.5, 0.5)) *
        img_size);  // sf::Vector2f(1.0, 1.0);
    shape[3].texCoords = to_Vector2f(
        (img_rect_center_ + size_ * scale_ * PIXEL_SIZE * Vec2d(-0.5, 0.5)) *
        img_size);  // sf::Vector2f(0.0, 1.0);

    // shape[0].texCoords = sf::Vector2f(0.0, 0.0);
    // shape[1].texCoords = sf::Vector2f(image_.getSize().x, 0.0);
    // shape[2].texCoords = sf::Vector2f(image_.getSize().x,
    // image_.getSize().y); shape[3].texCoords = sf::Vector2f(0.0,
    // image_.getSize().y);

    target.draw(shape, sf::RenderStates(&texture));
}
