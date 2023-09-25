/**
 * @file plot.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Graphical plot
 * @version 0.1
 * @date 2023-09-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef PLOT_H
#define PLOT_H

#include "data_structures/queue.hpp"
#include "renderable.h"

struct Plot : public Renderable {
    Plot(const Plot& plot) = default;
    Plot& operator=(const Plot& plot) = default;

    Plot(Vec2d center, Vec2d size, const char* label,
         unsigned point_count = 100);

    void add(double value);

    void render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                const AssetShelf& assets) override;

   private:
    double get_min() const;
    double get_max() const;

    const unsigned point_count_;

    Vec2d center_;
    Vec2d size_;

    const char* label_;

    Queue<double> values_;
};

#endif
