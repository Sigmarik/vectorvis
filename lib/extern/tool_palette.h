/**
 * @file tool_palette.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Tool palette and loader class
 * @version 0.1
 * @date 2023-12-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __EXTERN_TOOL_PALETTE_H
#define __EXTERN_TOOL_PALETTE_H

#include <stddef.h>

#include "Plug/Tool/Tool.h"
#include "data_structures/stack.h"

struct ToolPalette {
    static void addTool(plug::Tool& tool);
    static void selectTool(size_t id);
    static size_t selectionId() { return id_; }

    static size_t size();

    static plug::Tool& getTool();
    static plug::Tool& getTool(size_t index);

    static void cancel();

   private:
    ToolPalette() = default;

    static ToolPalette instance_;

    static Stack<plug::Tool*> tools_;

    static size_t id_;
};

#endif
