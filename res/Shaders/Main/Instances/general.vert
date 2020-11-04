#version 450

#define CASCADE_COUNT 4

//? vertex layout
layout (location = 0) in vec4 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec4 inCol;
layout (location = 3) in vec2 inTex;

//? out
layout (location = 0) out vec4  outCol;
layout (location = 1) out vec2  outTex;
layout (location = 2) out flat vec3 outSunDir;
layout (location = 3) out float outViewDistance;
layout (location = 4) out vec4  outShadowPos [CASCADE_COUNT];

//? meta
layout (binding = 0) uniform Meta 
{ 
    mat4  view;
    mat4  proj;
    vec4  viewDir; // set w to 1
    vec4  viewPos;
    float time;
} 
meta;

//? sun
layout(binding = 5) uniform Sun 
{ 
    mat4 projView       [CASCADE_COUNT];
    mat4 projViewBiased [CASCADE_COUNT];
    vec3 sunDir;
} 
sun;

//? instance data
struct General_InstanceData
{
    mat4 transform;
    mat4 padding;
};

layout (set = 0, binding = 1) readonly buffer General_InstanceDatas
{ 
    General_InstanceData sbo [];
} 
instanceDatas;

//? main
void main() 
{
    const General_InstanceData inst = instanceDatas.sbo[gl_InstanceIndex];
    gl_Position = meta.proj * meta.view * inst.transform * inPos;
    outCol = inCol;
    outTex = inTex;

    //shadow
    for(int i = 0; i < CASCADE_COUNT; ++i)
        outShadowPos[i] = sun.projViewBiased[i] * inst.transform * inPos;
    outSunDir = sun.sunDir;
    outViewDistance = distance(meta.viewPos.xyz, inPos.xyz);
}