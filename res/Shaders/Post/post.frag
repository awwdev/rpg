#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (binding = 0)  uniform sampler2D finalImage;

layout (location = 0) in  vec2 inTex;
layout (location = 1) in flat float inBlur;

layout (location = 0) out vec4 outCol;

void main() 
{
    float pi2 = 6.28318530718;
    float directions= 16;
    float scale     = 2;
    float steps     = 0.2f; 
    vec2 size = textureSize(finalImage, 0);

    if (inBlur == 1)
    {
        //? RADIAL BLUR
        vec3 col = texture(finalImage, inTex).rgb;
        for(float i = 0; i < pi2; i += pi2 / directions) {
        for(float j = 0.5; j < 1.5; j+=steps) {
            vec2 off = (vec2(cos(i), sin(i)) / size) * (j * scale);
            col += texture(finalImage, inTex + off).rgb;		
        }}
        col /= directions * (1 / steps);
        outCol = vec4(col.rgb, 1);
    }
    else 
    {
        outCol = vec4(texture(finalImage, inTex).rgb, 1);
    }




    //? SIMPLE BLOOM TEST
    float intensity = 0;
    scale = 2;
    for(float i = 0; i < pi2; i += pi2 / directions) {
    for(float j = 0.5; j < 1.5; j+=steps) {
        vec2 off = (vec2(cos(i), sin(i)) / size) * (j * scale);
        vec3 img = texture(finalImage, inTex + off).rgb;		
        intensity += img.r + img.g + img.b;
    }}
    intensity /= directions * (1 / steps);
    outCol.rgb += intensity / 10;

}