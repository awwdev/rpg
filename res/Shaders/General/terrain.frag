#version 450
#extension GL_ARB_separate_shader_objects : enable

#define CASCADE_COUNT 4

layout (location = 0) in vec4  inCol;
layout (location = 1) in vec2  inTex;
layout (location = 2) in float inShadowDot;
layout (location = 3) in vec4  inShadowCoord [CASCADE_COUNT];

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
    int cascadeIdx = 0;
    vec2 size = textureSize(shadowMap, 0).xy;

    //radial
    float directions = 8;
    float PI2 = 6.28318530718;
    float shadow = 0;
    for(float p = 0; p < PI2; p += PI2 / directions)
    {
        vec2 off   = vec2(sin(p), cos(p)) / size;
        vec4 coord = vec4(inShadowCoord[cascadeIdx].xy + off, 0, inShadowCoord[cascadeIdx].z);
        shadow    += texture(shadowMap, coord).r;
    }
    shadow /= directions;
    shadow = clamp(1 - shadow, 0, 1);

    #define AMBIENT 0.1f
    vec4 triangleColor = terrainTriangleColors.colors[gl_PrimitiveID];
    float shadowAmbient = clamp(AMBIENT + shadow, 0, 1); //inShadowDot
    vec3 colorFaceVertexBlend = inCol.rgb * (1 - triangleColor.a) + triangleColor.rgb * (triangleColor.a);
    outCol = vec4(colorFaceVertexBlend.rgb * shadowAmbient, 1);


}