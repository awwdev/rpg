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
    //vec2 size = textureSize(shadowMap, 0).xy;
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

    //shadow map

    //const float distNorm   = clamp(inViewDistance / 100.f, 0, 1);
    //const float distScaled = distNorm * 3;
    //const int   cascadeIdx = int(round(distScaled));

    //{ 
    //    const int   cascadeIdx = 0;
    //    const vec4  cascadeCoord = vec4(inShadowCoord[cascadeIdx].xy + 0, cascadeIdx, inShadowCoord[cascadeIdx].z);
    //    const float viewDist = clamp(inViewDistance / 5.f, 0, 1);
    //    const float cascadeDist = 1 - viewDist * 10 + 9;
    //    allCascadeShadows += texture(shadowMap, cascadeCoord).r * cascadeDist;    
    //}
    //{ 
    //    const int   cascadeIdx = 1;
    //    const vec4  cascadeCoord = vec4(inShadowCoord[cascadeIdx].xy + 0, cascadeIdx, inShadowCoord[cascadeIdx].z);
    //    const float viewDist = clamp((inViewDistance) / 10.f, 0, 1);
    //    const float cascadeDist = clamp(1 - abs(viewDist-0.5)* 4 + 1, 0, 1);
    //    allCascadeShadows += texture(shadowMap, cascadeCoord).r * cascadeDist;    
    //}

    const float shadowDistMax = 50.f;
    const float shadowDist = clamp(inViewDistance / shadowDistMax, 0, 1); //could be inside vert shader

    float allCascadeShadows = 0;
    for(int cascadeIdx = 0; cascadeIdx < CASCADE_COUNT; ++cascadeIdx)
    {
        const float f = 8; //cross fade factor (when segment linearly fades)
        const float s = CASCADE_COUNT; //segments
        const float i = cascadeIdx; //segment index
        const float x = shadowDist; //distance normalized [0,1][min,max]
        float y = 1 - abs(f*x - i * f/s - (f/8.f - 0.5)) + f/8.f - 0.5f;
        y = clamp(y, 0, 1);
       
        const vec4 cascadeCoord = vec4(inShadowCoord[cascadeIdx].xy + 0, cascadeIdx, inShadowCoord[cascadeIdx].z);
        allCascadeShadows += texture(shadowMap, cascadeCoord).r * y;    
    }
    allCascadeShadows = 1 - clamp(allCascadeShadows, 0, 1);
    

    
    

    //triangle face shadow
    const vec3 triangleNormal = terrainTriangleNormals.normals[gl_PrimitiveID];
    const vec3 sunDir = normalize(vec3(inSunDir.x, 0, inSunDir.z));
    const float shadowDot = 1 - min(1, max(0, dot(triangleNormal, sunDir)) * 15);

    //shadow sum
    const float AMBIENT = 0.1f;
    //const float shadowSum = clamp(AMBIENT + cascadeShadow * shadowDot, 0, 1);
    const float shadowSum = clamp(allCascadeShadows * shadowDot, 0, 1);

    //color
    const vec4 triangleColor = terrainTriangleColors.colors[gl_PrimitiveID];
    const vec3 colorSum = inCol.rgb * (1 - triangleColor.a) + triangleColor.rgb * (triangleColor.a);

    //out
    outCol = vec4(colorSum * shadowSum, 1);
}


//float allCascadeShadows = 0;
//for(int i = 0; i < CASCADE_COUNT; ++i)
//{
//     const vec4  cascadeCoord  = vec4(inShadowCoord[i].xy + 0, i, inShadowCoord[i].z);
//     const float cascadeShadow = texture(shadowMap, cascadeCoord).r;
//     allCascadeShadows += cascadeShadow;
//}    
//allCascadeShadows = 1 - min(1, allCascadeShadows);