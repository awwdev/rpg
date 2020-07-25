#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec4 inColors;
layout(location = 1) in vec4 inShadowCoord;
layout(binding  = 0) uniform sampler2D shadowMap;

void main() 
{
    float dist = texture(shadowMap, inShadowCoord.st).r;
    outColor = vec4(
        inColors.r * (1-dist), 
        inColors.g * (1-dist),
        inColors.b * (1-dist),
        inColors.a
    );
}