#include "image_view.h"

#include <string.h>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/OpenGL.hpp>

#include "gui/sf_cheatsheet.hpp"
#include "src/editor_tools/filter.h"
#include "src/editor_tools/tool.h"

ImageView::ImageView(ToolPalette* tools, FilterPalette* filters, Vec2d center,
                     Vec2d size, const char* file_name)
    : Interactive(center, size),
      texture_(),
      tools_(tools),
      filters_(filters),
      selection_(1, 1) {
    static sf::Image image;
    static sf::Texture texture;
    static sf::Sprite sprite;
    texture.loadFromFile(file_name);
    sprite.setTexture(texture, true);
    texture_.create(texture.getSize().x, texture.getSize().y);
    texture_.draw(sprite);
    texture_.setSmooth(false);
    update_image();
    selection_ = Mask(texture.getSize().x, texture.getSize().y);
}

ImageView::ImageView(ToolPalette* tools, FilterPalette* filters, Vec2d center,
                     Vec2d size, unsigned width, unsigned height)
    : Interactive(center, size),
      texture_(),
      tools_(tools),
      filters_(filters),
      selection_(width, height) {
    texture_.create(width, height);
    texture_.setSmooth(false);
    update_image();
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

    if (tools_ != nullptr && tools_->get_tool() != nullptr)
        tools_->get_tool()->render(stack, target, *this);
}

void ImageView::update_image() {
    texture_.display();
    image_ = texture_.getTexture().copyToImage();
}

void ImageView::update_from_image() {
    static sf::Sprite sprite;
    static sf::Texture texture;
    texture.loadFromImage(image_);
    sprite.setTexture(texture, true);
    texture_.draw(sprite);
    texture_.display();
}

void ImageView::upd_tool(const Vec2d& img_pos, const sf::Event& event) {
    if (filters_ != nullptr && event.type == sf::Event::KeyPressed) {
        bool correct_key = event.key.control &&
                           event.key.code <= sf::Keyboard::Num9 &&
                           event.key.code >= sf::Keyboard::Num0;

        unsigned id = (unsigned)(event.key.code - sf::Keyboard::Num0);

        bool correct_id = id < filters_->get_max_filter_count();

        if (correct_key && correct_id) {
            filters_->set_active_filter(id);

            update_image();
            filters_->get_active_filter()->apply(*this, selection_);
            update_from_image();
        }
    }

    if (tools_ == nullptr || tools_->get_tool() == nullptr) return;

    Tool& tool = *tools_->get_tool();

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

Mask::Mask(size_t width, size_t height)
    : data_(new bool[width * height]), width_(width), height_(height) {}

Mask::~Mask() {
    delete data_;
    width_ = height_ = 0;
}

Mask::Mask(const Mask& mask)
    : data_(new bool[mask.width_ * mask.height_]),
      width_(mask.width_),
      height_(mask.height_) {
    memcpy(data_, mask.data_, width_ * height_);
}

Mask& Mask::operator=(const Mask& mask) {
    delete data_;
    data_ = new bool[mask.width_ * mask.height_];
    width_ = mask.width_;
    height_ = mask.height_;
    return *this;
}

bool Mask::get_at(size_t pos_x, size_t pos_y) const {
    return data_[pos_x + pos_y * width_];
}

void Mask::set_at(size_t pos_x, size_t pos_y, bool value) {
    data_[pos_x + pos_y * width_] = value;
}

void Mask::fill(bool value) { memset(data_, value, width_ * height_); }

void Mask::invert() {
    for (size_t id = 0; id < width_ * height_; ++id) {
        data_[id] = !data_[id];
    }
}
