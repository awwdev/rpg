#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;
layout(binding  = 0) uniform sampler2D tex;

void main() {
    float r  = texture(tex, vec2(0, 0)).r;
    outColor = vec4(1.0, 0.0, 0.0, 1.0);
}