#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (binding  = 0) uniform sampler2D tex;

layout (location = 0) in vec2 inUV;
layout (location = 1) flat in int inBlur;

layout (location = 0) out vec4 outColor;

void main() 
{
    if (inBlur == 1)
    {
        outColor = texture(tex, inUV);
        vec2 size = textureSize(tex, 0);
        for(int x = -2; x <= 2; ++x) {
        for(int y = -2; y <= 2; ++y) {
            vec2 off = vec2(x / size.x, y / size.y);
            outColor += texture(tex, inUV + off);
        }}
        outColor /= 25;
    }
    else 
    {
        outColor = texture(tex, inUV);
    }
}
