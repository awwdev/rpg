#version 450

layout(push_constant) uniform Push {
    mat4 camera;
    mat4 sun;
    mat4 sunBias;
    vec3 sunDir;
} push;

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec4 inCol;
layout (location = 3) in vec4 inTex;
 
layout (location = 0) out vec4  outCol;
layout (location = 1) out vec4  outShadowCoord;
layout (location = 2) out float outShadowDot;

void main() 
{
    gl_Position = push.camera * inPos;
    outCol      = inCol;

    outShadowDot = dot(inNor, push.sunDir);
    outShadowDot = outShadowDot * 3; //"fade speed"
    outShadowDot = clamp(outShadowDot, 0, 1);

    outShadowCoord   = push.sunBias * inPos;
    outShadowCoord.w = 0;
}