#version 450

layout(location = 0) in float value1;
layout(location = 1) in float value2;

layout (location = 0) out vec2 coord;

layout(push_constant) uniform Push {
    float value;
} push;

vec2 positions[6] = vec2[](
    //vulkan: top is -y and bottom is +y
    vec2(-0.5, -0.5),
    vec2( 0.5,  0.5),
    vec2(-0.5,  0.5),

    vec2(-0.5, -0.5),
    vec2( 0.5, -0.5),
    vec2( 0.5,  0.5)
);

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
    float x = positions[gl_VertexIndex].x + push.value;
    float y = positions[gl_VertexIndex].y + value2;
    gl_Position = vec4(x, y, 0, 1.0);
}