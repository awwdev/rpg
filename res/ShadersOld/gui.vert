#version 450

struct INSTANCE_DATA {
    vec4 rect;
    uint colorIdx;
    uint textureIdx;
    uint padding1;
    uint padding2;
};

layout(binding = 1) uniform InstanceData { 
    INSTANCE_DATA arr[100];
} instanceData;

layout(binding = 2) uniform Colors { 
    vec4 colors [4];
} ubo_colors;

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
    vec2( 0,  0), //top left
    vec2( 1,  0), //top right
    vec2( 0,  1), //bot left
    vec2( 1,  0), //top right
    vec2( 1,  1), //bot right
    vec2( 0,  1), //bot left
};

void main() 
{
    const uint instID = gl_VertexIndex / 6;
    const uint vertID = gl_VertexIndex % 6;

    const INSTANCE_DATA instData = instanceData.arr[instID];

    const float vertOffX = quad[vertID].x * instData.rect.z;
    const float vertOffY = quad[vertID].y * instData.rect.w;

    const float x = instData.rect.X + vertOffX;
    const float y = instData.rect.y + vertOffY;
    gl_Position = vec4(x, y, 0, 1);

    outColors   = ubo_colors.colors[instanceData.arr[instID].colorIdx]; //color lookup
    outUV       = vec3(uv[vertID], instanceData.arr[instID].textureIdx); //texture index lookup
}