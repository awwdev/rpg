#version 450

layout(push_constant) uniform Push {
    mat4 camera;
    mat4 sun;
    vec3 sunDir;
} push;

layout (location = 0) out vec4 outCol;

void main() 
{
    gl_Position = vec4(0, 0, 0, 1);
    outCol      = vec4(1, 1, 1, 1);
}