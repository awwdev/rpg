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

//could do an UBO
const vec4 colors[] = {
    vec4(0.90, 0.90, 0.90, 1.0), //WHITE
    vec4(0.01, 0.90, 0.01, 1.0), //GREEN
    vec4(0.90, 0.01, 0.01, 1.0), //RED
    vec4(0.01, 0.01, 0.01, 1.0), //BLACK1
    vec4(0.02, 0.02, 0.02, 0.5), //BLACK2_ALPHA
    vec4(0.03, 0.03, 0.03, 1.0), //BLACK3
    vec4(0.04, 0.04, 0.04, 1.0), //BLACK4
    vec4(0.05, 0.05, 0.05, 1.0), //BLACK5
    vec4(235/255.f, 140/255.f, 52/255.f, 1.0), //ORANGE //235, 140, 52
};

void main() 
{
    const uint instID = gl_VertexIndex / 6;
    const uint vertID = gl_VertexIndex % 6;

    const INSTANCE_DATA instData = instanceData.arr[instID];

    const float vertOffX = quad[vertID].x * instData.rect.z;
    const float vertOffY = quad[vertID].y * instData.rect.w;

    const float x = instData.rect.x + vertOffX;
    const float y = instData.rect.y + vertOffY;
    gl_Position = vec4(x, y, 0, 1);

    outColors   = colors[instanceData.arr[instID].colorIdx]; //color lookup
    outUV       = vec3(uv[vertID], instanceData.arr[instID].textureIdx); //texture index lookup
}