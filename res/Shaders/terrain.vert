#version 450

//layout(push_constant) uniform Push {
//    mat4 camera;
//    mat4 sun;
//    mat4 sunBias;
//    vec3 sunDir;
//} push;

layout(push_constant) uniform Push {
    mat4 projection;
    mat4 view;
    mat4 sunView;
    vec3 sunDir;
    vec3 cascadeSplits; //array
} push;

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec4 inCol;
layout (location = 3) in vec4 inTex;
 
layout (location = 0) out vec4  outCol;
layout (location = 1) out vec4  outShadowCoord;
layout (location = 2) out float outShadowDot;
layout (location = 3) out vec3  outViewPos;
layout (location = 4) out vec3  outCascadeSplits;
layout (location = 5) out mat4  outSunView;
layout (location = 12) out vec4  outPos;





void main() 
{
    gl_Position  = push.projection * push.view * inPos;
    outCol       = inCol;

    outShadowDot = dot(inNor, push.sunDir);
    outShadowDot = outShadowDot * 3; //"fade speed"
    outShadowDot = clamp(outShadowDot, 0, 1);

    //float S = 0.001 + (cascadeIndex * 0.01);
    //float D = 0.00001f; 
    //float Z = 0.01f;
    //mat4 sunProj = mat4( 
    //    S, 0, 0, 0,
    //    0, S, 0, 0,
    //    0, 0, D, 0,
    //    0, 0, Z, 1 
    //);

    //outShadowCoord   = (biasMat * sunProj * push.sunView) * inPos;
    outSunView       = push.sunView;
    outPos           = inPos;
    outViewPos       = (push.view * vec4(inPos.xyz, 1)).xyz;
    outCascadeSplits = push.cascadeSplits;
}