#version 450

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec4 inCol;
layout (location = 3) in vec2 inTex;

layout(location = 0) out vec4 outCol;
layout(location = 1) out vec2 outTex;

void main() 
{
    gl_Position = inPos;
    outCol = inCol;
    outTex = inTex;
}