/**
 * @file molecule_view.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Graphics part of the particle simulation
 * @version 0.1
 * @date 2023-09-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MOLECULE_VIEW_H
#define MOLECULE_VIEW_H

#include "gui.h"
#include "src/simulation/simulation.h"

struct MoleculeView : public Renderable {
    MoleculeView(const GasVolume& volume, Vec2d center, Vec2d size);

    void render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                const AssetShelf& assets) override;

   private:
    Vec2d center_, size_;

    const GasVolume& volume_;
};

enum ValveType { VALVE_IN, VALVE_OUT };

struct ValveControlButton : public Button {
    ValveControlButton(GasVolume& volume, ValveType type, Vec2d center,
                       Vec2d size, const char* text);

    void on_push(Interaction interaction) override;
    void on_release(Interaction interaction) override;

   private:
    GasVolume& volume_;
    ValveType type_;
};

#endif
