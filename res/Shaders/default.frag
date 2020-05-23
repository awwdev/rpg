#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;
layout(binding  = 0) uniform sampler2D tex;

void main() {
    vec4 col = texture(tex, vec2(0, 0));
    outColor = vec4(col.r, col.g, col.b, 1);
}