// Texture atlas
uniform sampler2D atlas;

// Program timer
uniform float time;
// Time since the mouse hovered over the element (if negative, time since the
// button was unhovered)
uniform float hover_time;
// Time since the button was pushed (if negative, time since the button was
// released)
uniform float push_time;
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

void main() {
    vec2 coord = gl_TexCoord[0].xy;

    const float BORDER_THICKNESS = 1.2;

    int on_border = 0;
    if (coord.x <= BORDER_THICKNESS / size_px.x ||
        1.0 - coord.x <= BORDER_THICKNESS / size_px.x) {
        on_border = 1;
    }
    if (coord.y <= BORDER_THICKNESS / size_px.y ||
        1.0 - coord.y <= BORDER_THICKNESS / size_px.y) {
        on_border = 1;
    }

    float hover_transition = hover_time > 0.0 ? hover_time : 0.5 + hover_time;
    float push_transition =
        push_time > 0.0 ? push_time * 2.0 : 0.5 + push_time * 2.0;

    vec3 color = vec3(0.0, 0.0, 0.0);

    if (on_border != 0) {
        if (coord.x < push_transition || 1.0 - coord.x < push_transition)
            color = vec3(1.0, 1.0, 1.0);
        else
            color = vec3(0.0, 0.0, 0.0);
    } else {
        if (coord.x < push_transition || 1.0 - coord.x < push_transition)
            color = vec3(0.32, 0.33, 0.42) * 0.6;
        else {
            if (coord.y < hover_transition || 1.0 - coord.y < hover_transition)
                color = vec3(0.32, 0.33, 0.42) * 0.8;
            else
                color = vec3(0.32, 0.33, 0.42);
        }
    }

    gl_FragColor = vec4(color, 1.0);
}
