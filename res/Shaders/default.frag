#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 coord;

layout(location = 0) out vec4 outColor;
layout(binding  = 0) uniform sampler2D tex;

void main() {
    vec4 col = texture(tex, coord);
    outColor = vec4(1, 1, 1, 1);
}