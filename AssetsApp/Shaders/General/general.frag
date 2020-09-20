#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 inCol;
layout(location = 1) in vec2 inTex;

layout(location = 0) out vec4 outCol;

void main() 
{
    outCol = inCol;
}