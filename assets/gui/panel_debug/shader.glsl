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

void main() {
    vec2 coord = gl_TexCoord[0].xy;

    gl_FragColor =
        vec4(vec3(0.5 + coord.x * 0.5, 0.0, 0.5 + coord.y * 0.5), 1.0);
}
