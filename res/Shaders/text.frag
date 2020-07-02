#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding  = 0) uniform sampler2DArray tex;
layout(location = 0) out vec4 outColor;

layout(location = 0) in vec4 inColors;
layout(location = 1) in vec3 inUV;

void main() 
{
    outColor = vec4(1, 1, 1, texture(tex, inUV).b);
}