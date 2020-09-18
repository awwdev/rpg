#version 450

layout (location = 0) in vec2  inPos;
layout (location = 1) in vec2  inTex;
 
layout (location = 0) out vec2 outTex;

void main() 
{
    gl_Position = vec4(inPos, 0, 1);
    outTex = inTex;
}