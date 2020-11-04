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
struct Foliage_InstanceData
{
    mat4 transform;
    vec4 baseColor;
    vec4 padding [3];
};

layout (set = 0, binding = 2) readonly buffer Foliage_InstanceDatas
{ 
    Foliage_InstanceData sbo [];
} 
instanceDatas;

//? main
void main() 
{
    const Foliage_InstanceData inst = instanceDatas.sbo[gl_InstanceIndex];

    //using flow map
    float y = max(0, -inPos.y);
    vec4 vertPosMoved = vec4(
        inPos.x * (1 + sin(inPos.z + meta.time * 2.0) * 0.12 * y), 
        inPos.y * (1 + sin(          meta.time * 0.1) * 0.01 * y), 
        inPos.z * (1 + cos(inPos.x + meta.time * 1.5) * 0.15 * y), 
        1
    );

    gl_Position = meta.proj * meta.view * inst.transform * vertPosMoved;
    outCol = inst.baseColor * (1-y) + inCol * y;
    outTex = inTex;

    //shadow
    for(int i = 0; i < CASCADE_COUNT; ++i)
        outShadowPos[i] = sun.projViewBiased[i] * inst.transform * vertPosMoved;
    outSunDir = sun.sunDir;
    outViewDistance = distance(meta.viewPos.xyz, inPos.xyz);
}