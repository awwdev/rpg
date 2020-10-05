#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (binding = 0)  uniform sampler2D finalImage;

layout (location = 0) in  vec2 inTex;
layout (location = 1) in flat float inBlur;

layout (location = 0) out vec4 outCol;

void main() 
{
    if (inBlur == 1)
    {
        //? XY BLUR
        /*
        outCol = texture(finalImage, inTex);
        vec2 size = textureSize(finalImage, 0);
        const float scale = 1.5;
        for(int x = -2; x <= 2; ++x) {
        for(int y = -2; y <= 2; ++y) {
            vec2 off = vec2(x / size.x * scale, y / size.y * scale);
            outCol += texture(finalImage, inTex + off);
        }}
        outCol /= 25;
        */

        //? RADIAL BLUR
        const float pi2 = 6.28318530718;
        const float directions= 16;
        const float scale     = 2;
        const float steps     = 0.2f; 
        vec2 size = textureSize(finalImage, 0);

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
}