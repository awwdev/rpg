#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec4 inCol;
layout (location = 3) in vec2 inTex;

layout(binding = 0) uniform Meta { 
    mat4  view;
    mat4  proj;
    vec4  viewDir; //set w to 1
    vec4  viewPos; //set w to 1
    float time;
} meta;

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

layout(location = 0) out vec4 outCol;
layout(location = 1) out vec2 outTex;
layout(location = 2) out vec4 outShadowCoord [CASCADE_COUNT];

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
    mat4 metallic; //some settings
};

layout(binding = 1) readonly buffer MeshInstances { 
    MeshInstance arr [];
} meshInstances;

layout(binding = 2) uniform Sun { 
    mat4 projView       [CASCADE_COUNT];
    mat4 projViewBiased [CASCADE_COUNT];
} sun;

void main() 
{
    MeshInstance instance = meshInstances.arr[gl_InstanceIndex];

    //using flow map
    float y = max(0, -inPos.y);
    vec4 vertPosMoved = vec4(
        inPos.x * (1 + sin(inPos.z + meta.time * 2.0) * 0.12 * y), 
        inPos.y * (1 + sin(          meta.time * 0.1) * 0.01 * y), 
        inPos.z * (1 + cos(inPos.x + meta.time * 1.5) * 0.15 * y), 
        1
    );

    gl_Position = meta.proj * meta.view * instance.transform * vertPosMoved;
    outCol = inCol;
    outTex = inTex;

    for(int i = 0; i < CASCADE_COUNT; ++i){
        outShadowCoord[i] = sun.projViewBiased[i] * instance.transform * vertPosMoved;
        //also change array size of output and input in frag
    }
}
*/