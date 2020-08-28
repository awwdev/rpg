#version 450

layout(push_constant) uniform Push {
    mat4 camera;
    mat4 sun;
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

const mat4 biasMat = mat4( 
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.5, 0.5, 0.0, 1.0 
);

void main() 
{
    gl_Position = push.camera * instanceData.arr[gl_InstanceIndex].transform * inPos;
    outCol      = inCol;
    vec4 shadowCoords = (biasMat * push.sun) * (instanceData.arr[gl_InstanceIndex].transform * inPos);
    outShadowCoord    = shadowCoords;
    vec3 sunPos  = vec3(push.sun[3][0], push.sun[3][1], push.sun[3][2]);
    outShadowDot = dot(vec3(inNor), push.sunDir);
}