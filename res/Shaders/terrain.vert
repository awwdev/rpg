#version 450

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec4 inCol;
layout (location = 3) in vec4 inTex;
 
layout (location = 0) out vec4  outCol;
layout (location = 1) out vec4  outShadowCoord;
layout (location = 2) out float outShadowDot;
layout (location = 3) out vec4  outViewPos;
layout (location = 4) out vec4  outPos;

layout(push_constant) uniform Push {
    mat4 projection;
    mat4 view;
    mat4 sunView;
    vec3 sunDir;
} push;

layout (set = 0, binding = 1) uniform UBO {
	mat4 test;
} ubo;

void main() 
{
    gl_Position  = push.projection * push.view * inPos;
    outCol       = inCol;

    outShadowDot = dot(inNor, push.sunDir);
    outShadowDot = outShadowDot * 3; //"fade speed"
    outShadowDot = clamp(outShadowDot, 0, 1);

    outPos           = inPos;
    outViewPos       = push.view * inPos;
}