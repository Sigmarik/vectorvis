#include "render_control.h"

RenderControlButton::RenderControlButton(RayTracingPlane& controlled_plane,
                                         const Vec2d& center, const Vec2d& size,
                                         const char* text)
    : Button(center, size, text), plane_(controlled_plane) {}

void ProgressResetButton::on_push(Interaction interaction) {
    plane_.reset_progress();
}

void LightOnOffButton::on_push(Interaction interaction) {
    Vec3d& color = plane_.get_light(0).color;
    if (color.length2() <= 0.001) {
        color = Vec3d(1.0, 1.0, 1.0) * 7.0;
    } else {
        color = Vec3d(0.0, 0.0, 0.0);
    }
}
