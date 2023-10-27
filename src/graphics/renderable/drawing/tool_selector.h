/**
 * @file tool_selector.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Tool selector widget
 * @version 0.1
 * @date 2023-10-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef PALLET_WIDGET_H
#define PALLET_WIDGET_H

#include "src/editor_tools/tool.h"
#include "src/graphics/renderable/interactive.h"

struct ToolSelector : Interactive {
    ToolSelector(ToolPalette& pallet)
        : Interactive(Vec2d(0.0, 0.0), Vec2d(0.0, 0.0)), pallet_(pallet) {}

    void render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                const AssetShelf& assets) override {}

    void on_event(MatrixStack<Mat33d>& stack, Interaction interaction) override;

   private:
    ToolPalette& pallet_;
};

#endif
