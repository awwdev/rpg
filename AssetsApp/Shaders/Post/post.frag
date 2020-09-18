#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (binding = 0)  uniform sampler2D finalImage;

layout (location = 0) in  vec2 inTex;
layout (location = 0) out vec4 outCol;

void main() 
{
    outCol = texture(finalImage, inTex);
}