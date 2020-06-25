#version 450
//todo: shader extension to allow smaller types on INSTANCE_DATA

layout(push_constant) uniform Push {
    uint wnd_width;
    uint wnd_height;
} push;

struct INSTANCE_DATA {
    vec3 offset;
    uint colorIndex;
};

layout(binding = 1) uniform InstanceData { 
    INSTANCE_DATA arr[100];
} instanceData;


void main() 
{
    //vertId / 4 
    //vertId % 4

    float x = 0;//2 * (inPositions.x / push.wnd_width)  - 1;
    float y = 0;//2 * (inPositions.y / push.wnd_height) - 1;
    gl_Position = vec4(x, y, 0, 1);
}