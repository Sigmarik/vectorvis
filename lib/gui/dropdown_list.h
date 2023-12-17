/**
 * @file dropdown_list.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Dropdown list of elements
 * @version 0.1
 * @date 2023-12-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __GUI_DROPDOWN_LIST_H
#define __GUI_DROPDOWN_LIST_H

#include "gui.h"

struct DropdownList : public Button {
    DropdownList(plug::Widget& child, const plug::LayoutBox& box)
        : Button(box), child_(child){};

    void draw(plug::TransformStack& stack, plug::RenderTarget& target) override;

    void onPush() override;

    void onEvent(const plug::Event& event, plug::EHC& context) override;

    void onParentUpdate(const plug::LayoutBox& parent_box) override;

    virtual void onOpen(){};
    virtual void onClose(){};

   private:
    bool opened_ = false;

    plug::Widget& child_;
};

#endif
