#version 450
#extension GL_ARB_separate_shader_objects : enable

#define CASCADE_COUNT 4

layout(location = 0) in vec4 inCol;
layout(location = 1) in vec2 inTex;
layout(location = 2) in vec4 inShadowCoord [CASCADE_COUNT];

layout(location = 0) out vec4 outCol;

layout(binding  = 2) uniform sampler2DArrayShadow shadowMap;

void main() 
{
    float shadowSum = 0;
    for(int i = 0; i < CASCADE_COUNT; ++i){
        vec4  coord = vec4(inShadowCoord[i].xy, i, inShadowCoord[i].z);
        float shadow = texture(shadowMap, coord).r;
        shadowSum += shadow;
    }
    shadowSum /= CASCADE_COUNT;

    shadowSum = texture(shadowMap, vec4(inShadowCoord[0].xy, 0, inShadowCoord[0].z)).r;
    outCol = vec4(inCol.rgb * shadowSum, 1);
}