#version 450

//layout(location = 0) in float value1;
//layout(location = 1) in float value2;

layout (location = 0) out vec2 coord;

layout(push_constant) uniform Push {
    float value;
} push;

vec2 positions[6] = vec2[](
    vec2(-0.5, -0.5),
    vec2( 0.5,  0.5),
    vec2(-0.5,  0.5),

    vec2(-0.5, -0.5),
    vec2( 0.5, -0.5),
    vec2( 0.5,  0.5)
);

vec2 coords[6] = vec2[](
    vec2(0, 0),
    vec2(1, 1),
    vec2(0, 1),

    vec2(0, 0),
    vec2(1, 0),
    vec2(1, 1)
);

void main() {
    coord = coords[gl_VertexIndex];
    positions[gl_VertexIndex].x += push.value;
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
}