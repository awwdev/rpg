#version 450

#define CASCADE_COUNT 4

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec4 inCol;
layout (location = 3) in vec2 inTex;

layout(push_constant) uniform Push {
    uint cascadeIndex;
} push;

layout(binding = 0) uniform Sun { 
    mat4 projView       [CASCADE_COUNT];
    mat4 projViewBiased [CASCADE_COUNT];
} sun;

void main() 
{
    gl_Position = sun.projView[push.cascadeIndex] * inPos;
}