#version 450
//todo: shader extension to allow smaller types on INSTANCE_DATA

layout(push_constant) uniform Push {
    uint wnd_width;
    uint wnd_height;
} push;

struct INSTANCE_DATA {
    vec4 pos;
    vec2 size;
    uint colorIdx;
    uint textureIdx;
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

//alpha does not matter
const vec4 colors[] = {
    //font color
    vec4(0.9, 0.9, 0.9, 1),
    vec4(1.0, 0.0, 0.0, 1),
    //button background
    vec4(0.20, 0.20, 0.20 ,1),
    vec4(0.05, 0.05, 0.05 ,1),
    vec4(0.02, 0.02, 0.02 ,1),
    vec4(0.01, 0.01, 0.01 ,1),
};

void main() 
{
    const uint instID = gl_VertexIndex / 6;
    const uint vertID = gl_VertexIndex % 6;

    const INSTANCE_DATA instData = instanceData.arr[instID];

    const float quadX = instData.pos.x;
    const float quadY = instData.pos.y;
    const float quadW = instData.size.x;
    const float quadH = instData.size.y;

    const float vertOffX = quad[vertID].x * quadW;
    const float vertOffY = quad[vertID].y * quadH;

    const float xnorm1 = (quadX + vertOffX) / push.wnd_width;
    const float ynorm1 = (quadY + vertOffY) / push.wnd_height;
    const float xnorm2 = xnorm1 * 2 - 1;
    const float ynorm2 = ynorm1 * 2 - 1;



    gl_Position = vec4(xnorm2, ynorm2, 0, 1);
    outColors   = colors[instanceData.arr[instID].colorIdx]; //color lookup
    outUV       = vec3(uv[vertID], instanceData.arr[instID].textureIdx); //texture index lookup
}