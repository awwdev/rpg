#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 inColors;
layout(location = 1) in vec2 inCoords;

layout(location = 0) out vec4 outColor;

layout(binding  = 0) uniform sampler2D tex;
layout(binding  = 1) uniform TEXTURE_USAGE {
    bool use;
} textureUsage;

void main() {
    vec2 coords = vec2(inCoords.x / float(textureSize(tex, 0).x), inCoords.y / float(textureSize(tex, 0).y));
    vec4 col = texture(tex, coords);

    if (textureUsage.use == true){
        col = vec4( 1, 0, 0, 1 );
    }
    else {
        col = vec4( 0, 0, 1, 1 );
    }

    //col.r *= inColors.r;
    //col.g *= inColors.g;
    //col.b *= inColors.b;
    //col.a = inColors.a;
    outColor = col; 
}