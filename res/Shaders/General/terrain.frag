#version 450
#extension GL_ARB_separate_shader_objects : enable

#define CASCADE_COUNT 4

layout (location = 0) in vec4  inCol;
layout (location = 1) in vec2  inTex;
layout (location = 2) in flat vec3 inSunDir;
layout (location = 3) in float inViewDistance;
layout (location = 4) in vec4  inShadowCoord [CASCADE_COUNT];

layout (location = 0) out vec4 outCol;

layout (binding = 3) uniform sampler2DArrayShadow shadowMap;

//? terrain faces
layout (std430, binding = 6) readonly buffer TerrainTriangleNormals { 
    vec3 normals [];
} 
terrainTriangleNormals;

layout (std430, binding = 7) readonly buffer TerrainTriangleColors { 
    vec4 colors [];
} 
terrainTriangleColors;

void main() 
{
    //radial
    //float directions = 8;
    //float PI2 = 6.28318530718;
    //float shadow = 0;
    //for(float p = 0; p < PI2; p += PI2 / directions)
    //{
    //    vec2 off   = vec2(sin(p), cos(p)) / size;
    //    vec4 coord = vec4(inShadowCoord[cascadeIdx].xy + off, 0, inShadowCoord[cascadeIdx].z);
    //    shadow    += texture(shadowMap, coord).r;
    //}
    //shadow /= directions;
    //shadow = clamp(1 - shadow, 0, 1);

    const float distNorm = clamp(inViewDistance / 100.f, 0, 1);
    const float distScaled = (1 - distNorm) * 3;
    const int cascadeIdx = 0;//int(round(distScaled));
    vec2 size = textureSize(shadowMap, 0).xy;
    const vec2 off = vec2(0);
    const vec4 coord = vec4(inShadowCoord[cascadeIdx].xy + off, cascadeIdx, inShadowCoord[cascadeIdx].z);
    const float shadow = 1 - texture(shadowMap, coord).r;

    //triangle face shadow
    const vec3 triangleNormal = terrainTriangleNormals.normals[gl_PrimitiveID];
    const vec3 sunDir = normalize(vec3(inSunDir.x, 0, inSunDir.z));
    const float shadowDot = 1 - min(1, max(0, dot(triangleNormal, sunDir)) * 15);

    //shadow sum
    const float AMBIENT = 0.1f;
    const float shadowSum = clamp(AMBIENT + shadow * shadowDot, 0, 1);

    //color
    const vec4 triangleColor = terrainTriangleColors.colors[gl_PrimitiveID];
    const vec3 colorSum = inCol.rgb * (1 - triangleColor.a) + triangleColor.rgb * (triangleColor.a);

    //out
    outCol = vec4(colorSum * shadowSum, 1);
    //outCol = vec4(inViewDistance, inViewDistance, inViewDistance, 1);
}