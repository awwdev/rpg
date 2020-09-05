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

layout (set = 0, binding = 1) uniform UBO {
	mat4 camProj;
    mat4 camView;
    mat4 sunView;
    mat4 sunProjCasc [2];
    vec3 sunDir;
} ubo;

void main() 
{
    gl_Position  = ubo.camProj * ubo.camView * inPos;
    outCol       = inCol;

    outShadowDot = dot(inNor, ubo.sunDir);
    outShadowDot = outShadowDot * 3; //"fade speed"
    outShadowDot = clamp(outShadowDot, 0, 1);

    outPos       = inPos;
    outViewPos   = ubo.camView * inPos;
}