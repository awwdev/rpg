#version 450

layout(push_constant) uniform Push {
    mat4 camera;
    mat4 sun;
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
layout (location = 2) out vec4 outUV;

const mat4 biasMat = mat4( 
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.5, 0.5, 0.0, 1.0 
);

void main() 
{
    vec4 _pos = inPos;
    _pos.x += sin(push.time * 1.0) * _pos.y * 0.1;
    _pos.z += sin(push.time * 1.2) * _pos.y * 0.1;
    gl_Position = push.camera * instanceData.arr[gl_InstanceIndex].transform * _pos;

    outCol      = inCol;
    outUV       = inTex;
    
    vec4 shadowCoords = (biasMat * push.sun) * (instanceData.arr[gl_InstanceIndex].transform * _pos);
    outShadowCoord    = shadowCoords;
}