#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 inColors;
layout(location = 1) in vec2 inCoords;

layout(location = 0) out vec4 outColor;

layout(binding  = 0) uniform sampler2D tex;

void main() {
    vec2 coords = vec2(inCoords.x / float(textureSize(tex, 0).x), inCoords.y / float(textureSize(tex, 0).y));
    vec4 col = texture(tex, coords);
    //outColor = vec4(1, 1, 1, 1);
    outColor = col; 
}