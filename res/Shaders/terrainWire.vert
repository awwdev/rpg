#version 450

//layout(push_constant) uniform Push {
//    mat4 camera;
//    mat4 sun;
//} push;

layout(push_constant) uniform Push {
    mat4 projection;
    mat4 view;
    mat4 sunBiased;
    vec3 sunDir;
} push;

layout(location = 0) in vec4 inPos;
layout(location = 1) in vec4 inNor;
layout(location = 2) in vec4 inCol;
layout(location = 3) in vec4 inTex;

layout (location = 0) out vec4 outCol;

void main() 
{
    gl_Position = push.projection * push.view * inPos;
    outCol      = vec4(1, 1, 1, 0.5);
}