/**
 * @file popup_list.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Popup list element
 * @version 0.1
 * @date 2023-11-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef POPUP_LIST_H
#define POPUP_LIST_H

#include "src/graphics/renderable/gui.h"

struct PopupList : public Button {
    using Button::Button;

    void render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                const AssetShelf& assets) override;

    void on_event(MatrixStack<Mat33d>& stack, Interaction interaction) override;

    void set_child(Interactive& child) { child_ = child; }

   private:
    bool open_ = false;
    Interactive* child_ = nullptr;
};

#endif