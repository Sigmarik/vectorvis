#include "image_view.h"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/OpenGL.hpp>

#include "sf_cheatsheet.hpp"
#include "src/editor_tools/tool.h"

ImageView::ImageView(ToolPallet* pallet, Vec2d center, Vec2d size,
                     const char* file_name)
    : Interactive(center, size), texture_(), pallet_(pallet) {
    static sf::Image image;
    static sf::Texture texture;
    static sf::Sprite sprite;
    texture.loadFromFile(file_name);
    sprite.setTexture(texture, true);
    texture_.create(texture.getSize().x, texture.getSize().y);
    texture_.draw(sprite);
    texture_.setSmooth(false);
}

ImageView::ImageView(ToolPallet* pallet, Vec2d center, Vec2d size,
                     unsigned width, unsigned height)
    : Interactive(center, size), texture_(), pallet_(pallet) {
    texture_.create(width, height);
    texture_.setSmooth(false);
}

static const double PIXEL_SIZE = 0.1;

void ImageView::scroll_x(double position) {
    double low = size_.get_x() * scale_ * PIXEL_SIZE / 2.0;
    double high = 1.0 - size_.get_x() * scale_ * PIXEL_SIZE / 2.0;
    if (low > high) {
        img_rect_center_.set_x(0.5);
    } else {
        img_rect_center_.set_x(high * position + low * (1.0 - position));
    }
}

void ImageView::scroll_y(double position) {
    double low = size_.get_y() * scale_ * PIXEL_SIZE / 2.0;
    double high = 1.0 - size_.get_y() * scale_ * PIXEL_SIZE / 2.0;
    if (low > high) {
        img_rect_center_.set_y(0.5);
    } else {
        img_rect_center_.set_y(high * position + low * (1.0 - position));
    }
}

Vec2d ImageView::to_scene_coords(const Vec2d& pix_coord) const {
    Vec2d local = pix_coord *
                  Vec2d(1.0 / texture_.getSize().x, 1.0 / texture_.getSize().y);

    local = local - img_rect_center_;

    local = local / (scale_ * PIXEL_SIZE);

    return vis_size_ * Vec2d(-0.5, 0.5) * 0.0 + Vec2d(1.0, -1.0) * local;
}

void ImageView::on_event(MatrixStack<Mat33d>& stack, Interaction interaction) {
    sf::Vector2i mouse_pure = sf::Mouse::getPosition(*interaction.window);
    Vec2d mouse_pos =
        Vec2d((double)mouse_pure.x / interaction.window->getSize().x,
              (double)mouse_pure.y / interaction.window->getSize().y);

    sf::Event& event = *interaction.event;

    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Button::Left) {
        is_active_ = false;
    }

    if (interaction.obstructed || !is_under(stack, mouse_pos)) {
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Button::Left) {
        is_active_ = true;
    }

    if (is_active_ || true) {
        Vec3d mouse_rel = stack.top().inverse() *
                          Vec3d(mouse_pos.get_x(), mouse_pos.get_y(), 1.0);

        mouse_rel /= mouse_rel.get_z();

        Vec2d img_pos =
            img_rect_center_ + (Vec2d(mouse_rel.get_x(), -mouse_rel.get_y())) *
                                   (scale_ * PIXEL_SIZE);

        img_pos = img_pos * Vec2d(texture_.getSize().x, texture_.getSize().y);

        upd_tool(img_pos, event);
    }
}

static const float BORDER_COLOR[] = {0.0f, 0.0f, 0.0f, 0.2f};

void ImageView::render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                       const AssetShelf& assets) {
    static sf::VertexArray shape(sf::PrimitiveType::Quads, 4);

    texture_.display();

    Vec3d center = extrude(vis_center_);
    Vec3d size = extrude(vis_size_);

    // clang-format off
    shape[0].position = to_Vector2f(get_corner(TOP_LEFT,     stack));
    shape[1].position = to_Vector2f(get_corner(TOP_RIGHT,    stack));
    shape[2].position = to_Vector2f(get_corner(BOTTOM_RIGHT, stack));
    shape[3].position = to_Vector2f(get_corner(BOTTOM_LEFT,  stack));
    // clang-format on

    Vec2d img_size(texture_.getSize().x, texture_.getSize().y);

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

    sf::Texture::bind(&texture_.getTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, BORDER_COLOR);

    target.draw(shape, sf::RenderStates(&texture_.getTexture()));

    if (pallet_ != nullptr && pallet_->get_tool() != nullptr)
        pallet_->get_tool()->render(stack, target, *this);
}

void ImageView::upd_tool(const Vec2d& img_pos, const sf::Event& event) {
    if (pallet_ == nullptr || pallet_->get_tool() == nullptr) return;

    Tool& tool = *pallet_->get_tool();

    switch (event.type) {
        case sf::Event::MouseMoved: {
            tool.on_move(img_pos, *this);
        } break;
        case sf::Event::MouseButtonPressed: {
            switch (event.mouseButton.button) {
                case sf::Mouse::Left: {
                    tool.on_main(BTN_DOWN, img_pos, *this);
                } break;
                case sf::Mouse::Right: {
                    tool.on_sec(BTN_DOWN, img_pos, *this);
                } break;
                default:
                    break;
            }
        } break;
        case sf::Event::MouseButtonReleased: {
            switch (event.mouseButton.button) {
                case sf::Mouse::Left: {
                    tool.on_main(BTN_UP, img_pos, *this);
                } break;
                case sf::Mouse::Right: {
                    tool.on_sec(BTN_UP, img_pos, *this);
                } break;
                default:
                    break;
            }
        } break;
        case sf::Event::KeyPressed: {
            switch (event.key.code) {
                case sf::Keyboard::LShift: {
                    tool.on_modifier1(BTN_DOWN, img_pos, *this);
                } break;
                case sf::Keyboard::Enter: {
                    tool.confirm(*this);
                } break;
                case sf::Keyboard::Escape: {
                    tool.cancel();
                } break;
                default:
                    break;
            }
        } break;
        case sf::Event::KeyReleased: {
            switch (event.key.code) {
                case sf::Keyboard::LShift: {
                    tool.on_modifier1(BTN_UP, img_pos, *this);
                } break;
                default:
                    break;
            }
        } break;
        default:
            break;
    }
}
