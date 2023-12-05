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

#include "Impl/Widget.h"
#include "Plug/Tool.h"
#include "gui/anchor.h"

struct DebugToolData : public plug::PluginData {
    const char *getName(void) const override { return "Debug tool"; }

    const char *getTexturePath(void) const override { return nullptr; }
};

struct DebugTool : public plug::Tool {
    DebugTool() = default;
    DebugTool(const DebugTool &instance) = default;
    ~DebugTool() = default;

    DebugTool &operator=(const DebugTool &instance) = default;

    plug::Plugin *tryGetInterface(size_t interface_id) override;

    void addReference(void) override {}

    void release(void) override {}

    const plug::PluginData *getPluginData(void) const override;

    void setColorPalette(plug::ColorPalette &palette) override {}

    void setActiveCanvas(plug::Canvas &canvas) override { canvas_ = &canvas; }

    void onMainButton(const plug::ControlState &state,
                      const Vec2d &position) override;

    void onSecondaryButton(const plug::ControlState &state,
                           const Vec2d &position) override {}

    void onModifier1(const plug::ControlState &state) override {}

    void onModifier2(const plug::ControlState &state) override {}

    void onModifier3(const plug::ControlState &state) override {}

    void onMove(const Vec2d &position) override;

    void onConfirm(void) override {}

    void onCancel(void) override {}

    plug::Widget *getWidget() override;

   private:
    plug::Canvas *canvas_ = nullptr;

    void apply(const Vec2d &pos);

    bool active_ = false;

    static DebugTool instance_;
    static DebugToolData data_;

    Widget preview_ = Widget(Anchor(Vec2d(0.0, 0.0), Vec2d(100.0, 100.0),
                                    Vec2d(0.0, 0.0), Vec2d(0.0, 0.0)));
};

extern "C" plug::Plugin *loadPlugin(void);

#endif
