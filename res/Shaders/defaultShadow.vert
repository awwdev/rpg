#version 450

layout(push_constant) uniform Push {
    mat4 projection;
    uint wnd_width;
    uint wnd_height;
} push;

layout(location = 0) in vec4 inPos;
layout(location = 1) in vec4 inNor;
layout(location = 2) in vec4 inCol;
layout(location = 3) in vec4 inTex;

struct INSTANCE_DATA {
    mat4 transform;
};

layout(binding = 0) uniform InstanceData { 
    INSTANCE_DATA arr[1000];
} instanceData;


void main() 
{
    gl_Position = push.projection * instanceData.arr[gl_InstanceIndex].transform * inPos;
}