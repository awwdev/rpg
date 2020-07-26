#version 450

layout(push_constant) uniform Push {
    mat4 camera;
    mat4 sun;
} push;

layout(location = 0) in vec4 inPos;
layout(location = 1) in vec4 inNor;
layout(location = 2) in vec4 inCol;
layout(location = 3) in vec4 inTex;

void main() 
{
    gl_Position = push.sun * inPos;
}