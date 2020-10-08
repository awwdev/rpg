#version 450

#define CASCADE_COUNT 4

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec4 inCol;
layout (location = 3) in vec2 inTex;

layout(location = 0) out vec4  outCol;
layout(location = 1) out vec2  outTex;
layout(location = 2) out vec3  outNormal;
layout(location = 3) out vec3  outViewDir;
layout(location = 4) out flat int outMetallic;
layout(location = 5) out vec4  outShadowPos [CASCADE_COUNT];

layout(binding = 0) uniform Meta { 
    mat4  view;
    mat4  proj;
    vec3  viewDir;
    float time;
} meta;

struct MeshInstance 
{
    mat4 transform;
    mat4 metallic; //some settings
};

layout(std430, binding = 1) readonly buffer MeshInstances { 
    MeshInstance arr [];
} meshInstances;

layout(binding = 2) uniform Sun { 
    mat4 projView       [CASCADE_COUNT];
    mat4 projViewBiased [CASCADE_COUNT];
} sun;

void main() 
{
    MeshInstance instance = meshInstances.arr[gl_InstanceIndex];

    gl_Position = meta.proj * meta.view * instance.transform * vec4(inPos, 1);
    outCol = inCol;
    outTex = inTex;

    for(int i = 0; i < CASCADE_COUNT; ++i){
        outShadowPos[i] = sun.projViewBiased[i] * vec4(inPos, 1);
        //also change array size of output and input in frag
    }

    //metallic
    mat4 norMat = instance.transform;
    norMat[3][0] = 0;
    norMat[3][1] = 0;
    norMat[3][2] = 0;
    vec3 normal = normalize((norMat * vec4(inNor, 1)).xyz);

    outNormal = normal;
    outMetallic = int(instance.metallic[0][0]);
    outViewDir = normalize(meta.viewDir);
}