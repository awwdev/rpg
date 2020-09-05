#version 450

layout(push_constant) uniform Push {
    mat4 sunView;
    uint cascadeIndex;
} push;

layout(location = 0) in vec4 inPos;
layout(location = 1) in vec4 inNor;
layout(location = 2) in vec4 inCol;
layout(location = 3) in vec4 inTex;

void main() 
{
    float S;
    switch(push.cascadeIndex)
    {
        case 0: S = 0.005; break;
        case 1: S = 0.050; break;
        case 2: S = 0.100; break;
    }

    //float S = 0.003 + (push.cascadeIndex * 0.01);
    float D = 0.00001f; 
    float Z = 0.01f;
    mat4 sunProj = mat4( 
        S, 0, 0, 0,
        0, S, 0, 0,
        0, 0, D, 0,
        0, 0, Z, 1 
    );

    gl_Position = (sunProj * push.sunView) * inPos;
}