#version 450

layout(push_constant) uniform Push {
    mat4 camera;
    mat4 sun;
    mat4 sunBias;
    vec3 sunDir;
    float time;
} push;

//TODO: vec 3 would be more correct, compiler will add vec4 anyways but set w to 1, right?
layout(location = 0) in vec4 inPos;
layout(location = 1) in vec4 inNor;
layout(location = 2) in vec4 inCol;
layout(location = 3) in vec4 inTex;

struct INSTANCE_DATA {
    mat4 transform;
};

layout(binding = 0) uniform InstanceData { 
    INSTANCE_DATA arr[1000];
} instanceData;

layout (location = 0) out vec4 outCol;
layout (location = 1) out vec4 outShadowCoord;
layout (location = 2) out float outShadowDot;
layout (location = 3) out vec4 outUV;

void main() 
{
    vec4 _pos = inPos;
    _pos.x += sin(push.time * 1.0) * _pos.y * 0.1;
    _pos.z += sin(push.time * 1.2) * _pos.y * 0.1;

    gl_Position = push.camera * instanceData.arr[gl_InstanceIndex].transform * _pos;

    vec4 shadowCoords = push.sunBias * (instanceData.arr[gl_InstanceIndex].transform * _pos);
    outShadowCoord    = shadowCoords;

    outCol      = inCol;
    outUV       = inTex;
}