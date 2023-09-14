/**
 * @file render_control.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Render control buttons
 * @version 0.1
 * @date 2023-09-14
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef RENDER_CONTROL_BTN_H
#define RENDER_CONTROL_BTN_H

#include "../gui.h"
#include "../ray_tracing_plane.h"

struct RenderControlButton : public Button {
    RenderControlButton(RayTracingPlane& controlled_plane, const Vec2d& center,
                        const Vec2d& size, const char* text = "Button",
                        const Vec3d& color = DEFAULT_BUTTON_COLOR);

   protected:
    RayTracingPlane& plane_;
};

struct ProgressResetButton : public RenderControlButton {
    using RenderControlButton::RenderControlButton;

    void on_push(Interaction interaction) override;
};

struct LightOnOffButton : public RenderControlButton {
    using RenderControlButton::RenderControlButton;

    void on_push(Interaction interaction) override;
};

#endif
