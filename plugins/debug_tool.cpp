#include "debug_tool.h"

#include <stdio.h>

DebugTool DebugTool::instance_ = DebugTool();
DebugToolData DebugTool::data_ = DebugToolData();

plug::Plugin* DebugTool::tryGetInterface(size_t interface_id) {
    if (interface_id != (size_t)plug::PluginGuid::Tool) return nullptr;

    return &instance_;
}

const plug::PluginData* DebugTool::getPluginData(void) const { return &data_; }

void DebugTool::onMainButton(const plug::ControlState& state,
                             const Vec2d& position) {
    printf("Main button event at (%lg, %lg)\n", position.x, position.y);
}

void DebugTool::onMove(const Vec2d& position) {
    preview_.getLayoutBox().setPosition(position);
}

plug::Widget* DebugTool::getWidget() { return &preview_; }

plug::Plugin* loadPlugin(void) {
    static DebugTool tool;
    return &tool;
}
