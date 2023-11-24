#include "environment.h"

Vec2d Environment::screen_size_ = Vec2d(1.0, 1.0);

Environment& Environment::getInstance() {
    static Environment instance;
    return instance;
}
