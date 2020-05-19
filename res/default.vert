#version 450

//layout(location = 0) in float value1;
//layout(location = 1) in float value2;

layout(push_constant) uniform Push {
    float value;
} push;

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);


void main() {
    positions[gl_VertexIndex].x += push.value;
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
}