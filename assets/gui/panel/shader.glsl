// Texture atlas
uniform sampler2D atlas;

// Element space -> screen space matrix
uniform mat3 transform;

// Program timer
uniform float time;
// Coordinate of the center relative to the parent element
uniform vec2 center;
// Coordinate of the center relative to the window, in pixels
uniform vec2 center_px;
// Size of the button relative to the parent element
uniform vec2 size;
// Size of the button in pixels
uniform vec2 size_px;
// Mouse position relative to the window
uniform vec2 mouse;
// Window size (in pixels)
uniform vec2 window_size;

// gl_TexCoord[0].xy - pixel position relative to the button

float step(float left, float right, float slider) {
    if (slider < 0.0) return left;
    if (slider > 1.0) return right;
    return left + (right - left) *
                      (3.0 * slider * slider - 2.0 * slider * slider * slider);
}

vec2 to_screen(vec2 pos) {
    vec3 transformed = transform * vec3(pos, 1.0);
    vec2 pos_gl = transformed.xy / transformed.z;
    return pos_gl * window_size;
}

void main() {
    vec2 coord = gl_TexCoord[0].xy;

    const float BORDER_THICKNESS = 1.2;

    vec2 pixel_coord = center_px + size_px * (coord - 0.5);
    vec2 screen_coord =
        to_screen(center + size * (coord - 0.5) * vec2(1.0, -1.0));

    vec2 pos = pixel_coord * 0.1;

    float stripe_value = (pos.x + pos.y + time) * 0.3;
    bool stripe_mask = (stripe_value - floor(stripe_value)) > 0.5;
    float mouse_aura = max(0.0, 1.0 - length(screen_coord - mouse) / 100.0);
    float border_aura =
        max(0.0,
            1.0 - min(min(coord.x * size.x, coord.y * size.y),
                      min((1.0 - coord.x) * size.x, (1.0 - coord.y) * size.y)));

    int on_border = 0;
    if (coord.x <= BORDER_THICKNESS / size_px.x ||
        1.0 - coord.x <= BORDER_THICKNESS / size_px.x) {
        on_border = 1;
    }
    if (coord.y <= BORDER_THICKNESS / size_px.y ||
        1.0 - coord.y <= BORDER_THICKNESS / size_px.y) {
        on_border = 1;
    }

    vec3 color = vec3(0.0, 0.0, 0.0);

    if (on_border != 0)
        color = vec3(0, 0.0, 0.0);
    else
        color = vec3(0.22, 0.23, 0.32) * (stripe_mask ? 1.0 : 0.9) *
                step(0.9, 1.06, mouse_aura);

    color *= 1.0 - 0.2 * border_aura * border_aura * border_aura;

    gl_FragColor = vec4(color, 1.0);

    // gl_FragColor = texture2D(atlas, coord);
}
