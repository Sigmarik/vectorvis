/**
 * @file clock.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Analog c*ock
 * @version 0.1
 * @date 2023-10-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef CLOCK_H
#define CLOCK_H

#include "gui/interactive.h"

struct FpsMeter : public Interactive {
    FpsMeter(Vec2d center, double scale);

    void render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                const AssetShelf& assets) override;

    void tick() override;

   private:
    double scale_ = 1.0;

    unsigned long long last_tick_time_ = 0;
    unsigned long long delta_time_ = 1;
};

#endif
