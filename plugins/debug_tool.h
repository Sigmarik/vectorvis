/**
 * @file debug_tool.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Debug tool
 * @version 0.1
 * @date 2023-12-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __PLUGINS_DEBUG_TOOL_H
#define __PLUGINS_DEBUG_TOOL_H

#include "Plug/Tool.h"

struct DebugToolData : public plug::PluginData {
    const char *getName(void) const override { return "Debug tool"; }

    const char *getTexturePath(void) const override { return nullptr; }
};

struct DebugTool : public plug::Tool {
    DebugTool() = default;

    plug::Plugin *tryGetInterface(size_t interface_id) override;

    void addReference(void) override {}

    void release(void) override {}

    const plug::PluginData *getPluginData(void) const override;

    void setColorPalette(plug::ColorPalette &palette) override {}

    void setActiveCanvas(plug::Canvas &canvas) override {}

    void onMainButton(const plug::ControlState &state,
                      const Vec2d &position) override;

    void onSecondaryButton(const plug::ControlState &state,
                           const Vec2d &position) override {}

    void onModifier1(const plug::ControlState &state) override {}

    void onModifier2(const plug::ControlState &state) override {}

    void onModifier3(const plug::ControlState &state) override {}

    void onMove(const Vec2d &position) override {}

    void onConfirm(void) override {}

    void onCancel(void) override {}

    plug::Widget *getWidget() override { return nullptr; }

   private:
    static DebugTool instance_;
    static DebugToolData data_;
};

extern "C" plug::Plugin *loadPlugin(void);

#endif
