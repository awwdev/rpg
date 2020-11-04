#version 450

//? vertex layout
layout (location = 0) in vec4 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec4 inCol;
layout (location = 3) in vec2 inTex;

//? meta
layout (binding = 0) uniform Meta
{ 
    mat4  view;
    mat4  proj;
    vec4  viewDir; // set w to 1
    vec4  viewPos;
    float time;
} meta;

//? main
void main() 
{
    gl_Position = meta.proj * meta.view * inPos;
}