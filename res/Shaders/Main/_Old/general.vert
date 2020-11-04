#version 450

//? vertex layout

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec4 inCol;
layout (location = 3) in vec2 inTex;

//? meta

layout(binding = 0) uniform Meta 
{ 
    mat4  view;
    mat4  proj;
    vec4  viewDir; //set w to 1
    vec4  viewPos; //set w to 1
    float time;
} 
meta;

//? instance data

struct InstanceData_General
{
    mat4 transform;
};

struct InstanceData_Foliage
{
    mat4 transform;
};

struct InstanceData_Metallic
{
    mat4 transform;
};

struct InstanceData_Line
{
    mat4 transform;
};

//? instance ubo bindings

//TODO: keep set=0 meta
//TODO: rebind per material to set=1 binding=0
//TODO: did we use multiple sets for swapchain images?

layout(binding = 1) uniform General  { InstanceData_General  arr [100]; } general;
layout(binding = 2) uniform Foliage  { InstanceData_Foliage  arr [100]; } foliage;
layout(binding = 3) uniform Metallic { InstanceData_Metallic arr [100]; } metallic;
layout(binding = 4) uniform Line     { InstanceData_Line     arr [100]; } line;

void main() 
{
    gl_Position = meta.proj * meta.view * vec4(inPos, 1);
}

/*
#version 450

#define CASCADE_COUNT 4

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec4 inCol;
layout (location = 3) in vec2 inTex;

layout (location = 0) out vec4  outCol;
layout (location = 1) out vec2  outTex;
layout (location = 2) out flat vec3 outSunDir;
layout (location = 3) out float outViewDistance;
layout (location = 4) out vec4  outShadowPos [CASCADE_COUNT];
//layout(location = 2) out vec3  outNormal;
//layout(location = 3) out vec3  outViewDir;
//layout(location = 4) out flat int outMetallic;
//layout(location = 5) out flat int outGlow;
//layout(location = 6) out flat int outFlat;

layout(binding = 0) uniform Meta { 
    mat4  view;
    mat4  proj;
    vec4  viewDir; //set w to 1
    vec4  viewPos;
    float time;
} meta;

struct MeshInstance 
{
    mat4 transform;
    //
    float metallic;
    float glow;
    float flatShading;
    float placeholder;
    vec4 baseColor;
    vec4 placeholder2;
    vec4 placeholder3;
};

layout(std430, binding = 1) readonly buffer MeshInstances 
{ 
    MeshInstance arr [];
} 
meshInstances;

layout(binding = 2) uniform Sun { 
    mat4 projView       [CASCADE_COUNT];
    mat4 projViewBiased [CASCADE_COUNT];
} sun;

void main() 
{
    MeshInstance instance = meshInstances.arr[gl_InstanceIndex];

    //grass movement fun
    float y = max(0, -inPos.y);
    vec4 vertPosMoved = vec4(
        inPos.x * (1 + sin(inPos.z + meta.time * 2.0) * 0.12 * y), 
        inPos.y * (1 + sin(          meta.time * 0.1) * 0.01 * y), 
        inPos.z * (1 + cos(inPos.x + meta.time * 1.5) * 0.15 * y), 
        1
    );

    gl_Position = meta.proj * meta.view * instance.transform * vertPosMoved;//vec4(inPos, 1);
    outCol = instance.baseColor;
    outTex = inTex;

    for(int i = 0; i < CASCADE_COUNT; ++i){
        outShadowPos[i] = sun.projViewBiased[i] * instance.transform * vec4(inPos, 1);
        //also change array size of output and input in frag
    }

    outViewDistance = distance(meta.viewPos.xyz, inPos.xyz);

    //metallic
    //mat4 norMat = instance.transform;
    //norMat[3][0] = 0;
    //norMat[3][1] = 0;
    //norMat[3][2] = 0;
    //vec3 normal = normalize((norMat * vec4(inNor, 1)).xyz);
    //outNormal   = normal;
    //outMetallic = int(instance.settings[0][0]);
    //outViewDir  = normalize(meta.viewDir.xyz);
    //outGlow     = int(instance.settings[0][1]);
    //outFlat     = int(instance.settings[0][2]);
}
*/