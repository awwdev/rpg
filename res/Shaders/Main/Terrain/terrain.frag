#version 450
#extension GL_ARB_separate_shader_objects : enable

#define CASCADE_COUNT 4

//? in
layout (location = 0) in vec4  inCol;
layout (location = 1) in vec2  inTex;
layout (location = 2) in flat vec3 inSunDir;
layout (location = 3) in float inViewDistance;
layout (location = 4) in vec4  inShadowCoord [CASCADE_COUNT];

layout (location = 0) out vec4 outCol;

//? bindings
layout (binding = 6) uniform sampler2DArrayShadow shadowMap;
layout (std430, binding = 7) readonly buffer TerrainTriangleNormals 
{ 
    vec3 normals [];
} 
terrainTriangleNormals;

layout (std430, binding = 8) readonly buffer TerrainTriangleColors 
{ 
    vec4 colors [];
} 
terrainTriangleColors;

//? main
void main() 
{
    const vec2 size = textureSize(shadowMap, 0).xy;

    //shadow distance mapping
    const float shadowDistMax = 300.f;
    const float shadowDistNorm = clamp(inViewDistance / shadowDistMax, 0, 1); //could be inside vert shader
    const float shadowDist = clamp(-pow(shadowDistNorm-1, 4) + 1, 0, 1); //issue: will spend lots of time outfading in last casc

    //cascade sums
    float allCascadeShadows = 0;
    for(int cascadeIdx = 0; cascadeIdx < CASCADE_COUNT; ++cascadeIdx)
    {
        //shadow cascade "usage factor"
        //https://www.desmos.com/calculator/ua5dzr7hfp
        const float f = 6; //cross fade factor (when segment linearly fades)
        const float s = CASCADE_COUNT; //segments
        const float i = cascadeIdx; //segment index
        const float x = shadowDist; //distance normalized [0,1][min,max]
        float y = 1 - abs(f*x - i * f/s - (f/8.f - 0.5)) + f/8.f - 0.5f; //abs
        y = clamp(y, 0, 1);

        //radial
        const float directions = 8;
        const float PI2 = 6.28318530718;
        float radial = 0;
        for(float p = 0; p < PI2; p += PI2 / directions)
        {
            vec2 off   = vec2(sin(p), cos(p)) / size;
            vec4 coord = vec4(inShadowCoord[cascadeIdx].xy + off, cascadeIdx, inShadowCoord[cascadeIdx].z);
            radial    += texture(shadowMap, coord).r;
        }
        radial /= directions;
        allCascadeShadows += radial;

        //without radial:
        //const vec4 cascadeCoord = vec4(inShadowCoord[cascadeIdx].xy + 0, cascadeIdx, inShadowCoord[cascadeIdx].z);
        //allCascadeShadows += texture(shadowMap, cascadeCoord).r * y;    
    }
    allCascadeShadows = 1 - clamp(allCascadeShadows, 0, 1);
    
    //triangle face shadow
    const vec3 triangleNormal = terrainTriangleNormals.normals[gl_PrimitiveID];
    const vec3 sunDir = normalize(vec3(inSunDir.x, 0, inSunDir.z));
    const float shadowDot = 1 - min(1, max(0, dot(triangleNormal, sunDir)) * 15);

    //shadow sum
    const float AMBIENT = 0.1f;
    const float shadowSum = clamp(AMBIENT + allCascadeShadows * shadowDot, 0, 1);

    //color
    const vec4 triangleColor = terrainTriangleColors.colors[gl_PrimitiveID];
    const vec3 colorSum = inCol.rgb * (1 - triangleColor.a) + triangleColor.rgb * (triangleColor.a);

    //out
    outCol = vec4(colorSum * shadowSum, 1);
}