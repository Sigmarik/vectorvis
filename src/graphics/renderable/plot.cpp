#include "plot.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "sf_cheatsheet.hpp"

Plot::Plot(Vec2d center, Vec2d size, const char* label, unsigned point_count)
    : point_count_(point_count),
      center_(center),
      size_(size),
      label_(label),
      values_() {}

void Plot::add(double value) {
    values_.push(value);
    if (values_.size() > point_count_) values_.pop();
}

void Plot::render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                  const AssetShelf& assets) {
    static sf::Text render_text("[DEBUG TEXT]", sf::Font(), 13);
    static sf::VertexArray plot(sf::PrimitiveType::LinesStrip, point_count_);

    double low = get_min();
    double high = get_max();

    for (unsigned id = 0; id < point_count_; ++id) {
        double val = 0.0;
        if (id < values_.size()) val = values_[id];

        plot[id].position = to_Vector2f(
            stack.top() *
            extrude(center_ +
                    size_ * Vec2d((double)id / (double)point_count_ - 0.5,
                                  (val) / (high - low) - 0.5)));
        plot[id].color = sf::Color(200, 255, 200);
    }

    target.draw(plot);

    double text_scale_x = (stack.top() * Vec3d(1.0, 0.0, 0.0)).length() / 50.0;
    double text_scale_y = (stack.top() * Vec3d(0.0, 1.0, 0.0)).length() / 50.0;

    render_text.setScale(
        sf::Vector2f((float)text_scale_x, (float)text_scale_y));

    render_text.setFont(assets.font);

    render_text.setString(label_);
    render_text.setOrigin(
        sf::Vector2f(0.0, render_text.getLocalBounds().height));
    render_text.setPosition(to_Vector2f(
        stack.top() * extrude(center_ + size_ * Vec2d(-0.5, -0.5))));

    target.draw(render_text);

    static char value_string[64] = "";
    double value = values_.size() ? values_[values_.size() - 1] : 0.0;
    sprintf(value_string, "%.2lg", value);
    render_text.setString(value_string);
    render_text.setOrigin(sf::Vector2f(render_text.getLocalBounds().width,
                                       render_text.getLocalBounds().height));
    render_text.setPosition(
        to_Vector2f(stack.top() * extrude(center_ + size_ * Vec2d(0.5, -0.5))));

    target.draw(render_text);

    plot.resize(point_count_);
}

double Plot::get_min() const {
    double low = -0.01;
    for (size_t id = 0; id < values_.size(); ++id) {
        low = low < values_[id] ? low : values_[id];
    }
    return low;
}

double Plot::get_max() const {
    double high = 0.01;
    for (size_t id = 0; id < values_.size(); ++id) {
        high = high > values_[id] ? high : values_[id];
    }
    return high;
}
