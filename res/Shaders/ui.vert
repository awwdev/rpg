#version 450

//PER VERTEX DATA
layout(location = 0) in vec3 inPositions;
layout(location = 1) in vec4 inColors;
layout(location = 2) in vec2 inCoords;

//PER INSTANCE DATA
layout(location = 3) in vec2 instOffsets;
layout(location = 4) in vec4 instColors;

layout (location = 0) out vec4 outColors;
layout (location = 1) out vec2 outCoords;

layout(push_constant) uniform Push {
    uint wnd_width;
    uint wnd_height;
} push;

void main() {
    outCoords = inCoords;
    //outColors = inColors;
    outColors = instColors;

    float x = 2 * (inPositions.x / push.wnd_width)  - 1;
    float y = 2 * (inPositions.y / push.wnd_height) - 1;
    //gl_Position = vec4(x, y, 0, 1.0);

    gl_Position = vec4(instOffsets.x / push.wnd_width + x, instOffsets.y / push.wnd_height + y, 0, 1.0);
}