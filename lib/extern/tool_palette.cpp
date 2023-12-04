#include "tool_palette.h"

#include <assert.h>

ToolPalette ToolPalette::instance_ = ToolPalette();

Stack<plug::Tool*> ToolPalette::tools_ = Stack<plug::Tool*>();

size_t ToolPalette::id_ = 0;

void ToolPalette::addTool(plug::Tool& tool) { tools_.push(&tool); }

void ToolPalette::selectTool(size_t id) {
    assert(id < tools_.size());

    id_ = id;
}

size_t ToolPalette::size() { return tools_.size(); }

plug::Tool& ToolPalette::getTool() {
    assert(id_ < tools_.size());

    return *tools_[id_];
}

plug::Tool& ToolPalette::getTool(size_t index) {
    assert(index < size());

    return *tools_[index];
}

void ToolPalette::cancel() {
    for (size_t id = 0; id < size(); ++id) {
        tools_[id]->onCancel();
    }
}
