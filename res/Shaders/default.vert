#version 450

layout(location = 0) in vec3 inPositions;
layout(location = 1) in vec4 inColors;

layout (location = 0) out vec2 coord;
layout (location = 1) out vec4 outColors;

layout(push_constant) uniform Push {
    float value;
} push;

vec2 coords[6] = vec2[](
    //texture is y0 is bottom and y1 is top
    vec2(0, 1),
    vec2(1, 0),
    vec2(0, 0),

    vec2(0, 1),
    vec2(1, 1),
    vec2(1, 0)
);

void main() {
    coord = coords[gl_VertexIndex];
    outColors = inColors;

    float x = inPositions.x + push.value;
    float y = inPositions.y;
    gl_Position = vec4(x, y, 0, 1.0);
}