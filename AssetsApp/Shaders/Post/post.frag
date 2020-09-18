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
        outCol = texture(finalImage, inTex);
        vec2 size = textureSize(finalImage, 0);
        const float scale = 2;
        for(int x = -2; x <= 2; ++x) {
        for(int y = -2; y <= 2; ++y) {
            vec2 off = vec2(x / size.x * scale, y / size.y * scale);
            outCol += texture(finalImage, inTex + off);
        }}
        outCol /= 25;
    }
    else 
    {
        outCol = texture(finalImage, inTex);
    }

}