#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 inColors;
layout(location = 1) in vec2 inCoords;

layout(location = 0) out vec4 outColor;

layout(binding  = 0) uniform sampler2D tex;

void main() {
    vec2 coords = vec2(inCoords.x / float(textureSize(tex, 0).x), inCoords.y / float(textureSize(tex, 0).y));
    vec4 col = texture(tex, coords);
    col.r *= inColors.r;
    col.g *= inColors.g;
    col.b *= inColors.b;
    col.a = inColors.a;
    outColor = col; 
}