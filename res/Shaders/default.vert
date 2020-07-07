#version 450
//todo: shader extension to allow smaller types on INSTANCE_DATA

layout(push_constant) uniform Push {
    uint wnd_width;
    uint wnd_height;
} push;

layout (location = 0) out vec4 outColors;

void main() 
{
    gl_Position = vec4(0, 0, 0, 0);
    outColors   = vec4(1, 1, 1, 1);
}