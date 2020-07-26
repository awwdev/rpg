#version 450

layout(push_constant) uniform Push {
    mat4 camera;
    mat4 sun;
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

    vec4 shadowCoords = (biasMat * push.sun) * inPos;
    outShadowCoord    = shadowCoords;
}