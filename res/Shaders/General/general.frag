#version 450
#extension GL_ARB_separate_shader_objects : enable

#define CASCADE_COUNT 4

layout (location = 0) in vec4  inCol;
layout (location = 1) in vec2  inTex;
layout (location = 2) in flat vec3 inSunDir;
layout (location = 3) in float inViewDistance;
layout (location = 4) in vec4  inShadowCoord [CASCADE_COUNT];

layout(location = 0) out vec4 outCol;

layout(binding  = 3) uniform sampler2DArrayShadow shadowMap;
layout(binding  = 5) uniform sampler2DArray fxTextures;

void main() 
{
    //int cascadeIdx = 0;
    //vec2 size = textureSize(shadowMap, 0).xy;
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
    //#define AMBIENT 0.1f
    //float shadowAmbient = clamp(AMBIENT + shadow, 0, 1);
    //const vec3 lightDir = vec3(1, 1, 1);
    //float lambertian = max(dot(lightDir, inNormal), 0.0);
    //float specular = 1;
    //if(lambertian > 0.0) {
    //   vec3 reflectDir = reflect(-lightDir, inNormal);
    //   float specAngle = max(dot(reflectDir, -inViewDir), 0.0);
    //   specular = pow(specAngle, 4.0);
    //}
    //vec3  voronoi1 = texture(fxTextures, vec3(inTex * 3.5, 0)).rgb;
    //float voronoi2 = abs(sin(dot(voronoi1, inNormal)));
    //outCol = vec4( 
    //    inCol.rgb * max(0.5, 1 - inMetallic) 
    //    * shadowAmbient 
    //    * (1 + inMetallic * voronoi2)
    //, 1);
    //if (inFlat > 0)
    //{
    //    outCol = inCol;
    //}
    //if (inGlow > 0)
    //{
    //    outCol.r = 1;
    //    outCol.g = 1;
    //    outCol.b = 1;
    //}

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
    
    //shadow sum
    const float AMBIENT = 0.1f;
    const float shadowSum = clamp(AMBIENT + allCascadeShadows, 0, 1);

    outCol = vec4(inCol.rgb * shadowSum, 1);
}