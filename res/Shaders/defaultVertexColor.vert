#version 450

layout(push_constant) uniform Push {
    mat4 camera;
    mat4 sun;
    mat4 sunBias;
    vec3 sunDir;
} push;

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

void main() 
{
    gl_Position = push.camera * instanceData.arr[gl_InstanceIndex].transform * inPos;
    outCol      = inCol;
    vec4 shadowCoords = push.sunBias * (instanceData.arr[gl_InstanceIndex].transform * inPos);
    outShadowCoord    = shadowCoords;
    
    vec3 sunPos  = vec3(push.sun[3][0], push.sun[3][1], push.sun[3][2]);

    mat4 rot = instanceData.arr[gl_InstanceIndex].transform;
    rot[3][0] = 0;
    rot[3][1] = 0;
    rot[3][2] = 0;

    vec4 nor = normalize(rot * inNor);
    outShadowDot = dot(vec3(nor), push.sunDir);
    outShadowDot = (outShadowDot * 1000);
    outShadowDot = clamp(outShadowDot, 0, 1);
}