#version 450

layout(location = 0) in vec3 inPositions;

layout(push_constant) uniform Push {
    uint wnd_width;
    uint wnd_height;
} push;

void main() {
    float x = 2 * (inPositions.x / push.wnd_width)  - 1;
    float y = 2 * (inPositions.y / push.wnd_height) - 1;
    gl_Position = vec4(x, y, 0, 1.0);
}