#include "world_timer.h"

sf::Clock WorldTimer::clock_;
sf::Clock WorldTimer::tick_clock_;

unsigned long long WorldTimer::get() {
    return (unsigned long long)clock_.getElapsedTime().asMicroseconds();
}

double WorldTimer::get_delta() {
    double delta = tick_clock_.getElapsedTime().asSeconds();
    tick_clock_.restart();
    return delta;
}

WorldTimer::WorldTimer() {
    clock_ = sf::Clock();
    tick_clock_ = sf::Clock();
}
