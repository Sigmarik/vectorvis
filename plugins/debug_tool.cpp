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
    if ((state.state == plug::State::Pressed)) {
        apply(position);
        active_ = true;
    } else {
        active_ = false;
    }
}

void DebugTool::onMove(const Vec2d& position) {
    preview_.getLayoutBox().setPosition(position);

    if (active_) {
        apply(position);
    }
}

plug::Widget* DebugTool::getWidget() { return &preview_; }

void DebugTool::apply(const Vec2d& pos) {
    if (canvas_ == nullptr) {
        printf("Canvas has not been set!\n");
        return;
    }

    long long pos_x = (long long)pos.x;
    long long pos_y = (long long)pos.y;

    size_t lim_x = (size_t)canvas_->getSize().x;
    size_t lim_y = (size_t)canvas_->getSize().y;

    int red = 0, green = 0, blue = 0;

    for (int dx = -50; dx <= 50; ++dx) {
        for (int dy = -50; dy <= 50; ++dy) {
            if (dx * dx + dy * dy > 50 * 50) continue;

            long long cell_x = pos_x + dx;
            long long cell_y = pos_y + dy;
            if (cell_x >= (long long)lim_x || cell_x < 0) continue;
            if (cell_y >= (long long)lim_y || cell_y < 0) continue;

            plug::Color color =
                canvas_->getPixel((size_t)cell_x, (size_t)cell_y);
            canvas_->setPixel(
                (size_t)cell_x, (size_t)cell_y,
                plug::Color(color.r / 2, color.g / 2, color.b / 2));
        }
    }
}

plug::Plugin* loadPlugin(void) {
    static DebugTool tool;
    return &tool;
}
