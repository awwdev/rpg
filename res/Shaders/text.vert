#version 450
//todo: shader extension to allow smaller types on INSTANCE_DATA

layout(push_constant) uniform Push {
    uint wnd_width;
    uint wnd_height;
} push;

struct INSTANCE_DATA {
    vec4  offset;
    uint  colorIdx;
    uint  textureIdx;
    //here is padding involved
};

layout(binding = 1) uniform InstanceData { 
    INSTANCE_DATA arr[100];
} instanceData;

layout (location = 0) out vec4 outColors;
layout (location = 1) out vec3 outUV;

const vec2 quad [6] = {
    vec2( 0,  0), //top left
    vec2( 1,  0), //top right
    vec2( 0,  1), //bot left
    vec2( 1,  0), //top right
    vec2( 1,  1), //bot right
    vec2( 0,  1), //bot left
};

const vec2 uv [6] = {
    vec2( 0,  1), //top left
    vec2( 1,  1), //top right
    vec2( 0,  0), //bot left
    vec2( 1,  1), //top right
    vec2( 1,  0), //bot right
    vec2( 0,  0), //bot left
};

const vec4 colors[4] = {
    vec4(1, 1, 1, 1),
    vec4(1, 0, 0, 1),
    vec4(0, 1, 0, 1),
    vec4(0, 0, 1, 1),
};

void main() 
{
    const uint instID = gl_VertexIndex / 6;
    const uint vertID = gl_VertexIndex % 6;

    //TODO: size of letter
    const float size = 32;

    //vert position
    float x = 2 * ((quad[vertID].x * size) / push.wnd_width ) - 1;
    float y = 2 * ((quad[vertID].y * size) / push.wnd_height) - 1;
    //quad offest
    x += 2 * instanceData.arr[instID].offset.x / push.wnd_width;
    y += 2 * instanceData.arr[instID].offset.y / push.wnd_height;

    gl_Position = vec4(x, y, 0, 1);
    outColors   = colors[instanceData.arr[instID].colorIdx]; //color lookup
    outUV       = vec3(uv[vertID], instanceData.arr[instID].textureIdx); //texture index lookup
}