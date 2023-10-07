#include "fps_meter.h"

#include <math.h>

#include <SFML/Graphics/Text.hpp>

#include "src/graphics/renderable/sf_cheatsheet.hpp"

static double smoothstep(double alpha, double beta, double param) {
    if (param < 0.0) return alpha;
    if (param > 1.0) return beta;
    double new_param = 3.0 * param * param - 2.0 * param * param;
    return lerp(alpha, beta, new_param);
}

static sf::Color lerp(sf::Color alpha, sf::Color beta, double param) {
    return sf::Color((sf::Uint8)smoothstep(alpha.r, beta.r, param),
                     (sf::Uint8)smoothstep(alpha.g, beta.g, param),
                     (sf::Uint8)smoothstep(alpha.b, beta.b, param));
}

FpsMeter::FpsMeter(Vec2d center, double scale)
    : Interactive(center, Vec2d(1.0, 0.2)), scale_(scale) {}

void FpsMeter::render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                      const AssetShelf& assets) {
    static sf::Text text;

    text.setFont(assets.font);
    text.setCharacterSize(12);

    double text_scale_x = (stack.top() * Vec3d(1.0, 0.0, 0.0)).length() / 50.0;
    double text_scale_y = (stack.top() * Vec3d(0.0, 1.0, 0.0)).length() / 50.0;

    text.setScale(sf::Vector2f((float)(text_scale_x * scale_),
                               (float)(text_scale_y * scale_)));
    text.setPosition(to_Vector2f(stack.top() * extrude(vis_center_)));

    double fps = 1e6 / (double)(delta_time_ > 0 ? delta_time_ : 1);

    text.setColor(lerp(sf::Color::Red, sf::Color::Green, fps / 100.0));

    static char fps_string[32] = "";

    sprintf(fps_string, "%.1lf", fps);

    text.setString(fps_string);

    text.setOrigin(sf::Vector2f(text.getLocalBounds().width,
                                text.getLocalBounds().height / 2.0f));

    target.draw(text);
}

void FpsMeter::tick() {
    unsigned long long time = WorldTimer::get();

    delta_time_ = time - last_tick_time_;

    last_tick_time_ = time;
}
