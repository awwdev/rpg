#version 450

#define CASCADE_COUNT 4

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec4 inCol;
layout (location = 3) in vec2 inTex;

layout(location = 0) out vec4 outCol;
layout(location = 1) out vec2 outTex;
layout(location = 2) out vec4 outShadowPos [CASCADE_COUNT];

layout(binding = 0) uniform Meta { 
    mat4 view;
    mat4 proj;
} meta;

layout(binding = 1) uniform Models { 
    mat4 transform;
} models;

layout(binding = 2) uniform Sun { 
    mat4 projView       [CASCADE_COUNT];
    mat4 projViewBiased [CASCADE_COUNT];
} sun;

void main() 
{
    gl_Position = meta.proj * meta.view * models.transform * inPos;
    outCol = inCol;
    outTex = inTex;

    for(int i = 0; i < CASCADE_COUNT; ++i){
        outShadowPos[i] = sun.projViewBiased[i] * inPos;
        //also change array size of output and input in frag
    }
}