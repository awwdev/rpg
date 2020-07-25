#version 450

layout(push_constant) uniform Push {
    mat4 camera;
    mat4 sun;
} push;

layout(location = 0) in vec4 inPos;
layout(location = 1) in vec4 inNor;
layout(location = 2) in vec4 inCol;
layout(location = 3) in vec4 inTex;

layout (location = 0) out vec4 outCol;

void main() 
{
    gl_Position = push.camera * inPos;
    outCol      = vec4(1, 1, 1, 0.5);
}