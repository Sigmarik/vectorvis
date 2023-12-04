#include "tool_selector_btn.h"

#include "extern/tool_palette.h"

void ToolSelectionButton::onPush() { ToolPalette::selectTool(id_); }

void ToolSelectionButton::draw(plug::TransformStack& stack,
                               plug::RenderTarget& target) {
    if (ToolPalette::selectionId() == id_ && !isPushed()) {
        setDesign(DSGN_PANEL_RED_HIGHLIGHT);
    }

    Button::draw(stack, target);
}
