#version 450

const vec4 pos [] = 
{
    vec4( 0.0,-0.5, 0, 1),
    vec4( 0.5, 0.5, 0, 1),
    vec4(-0.5, 0.5, 0, 1),
};

const vec4 col [] = 
{
    vec4(1, 0, 0, 1),
    vec4(0, 1, 0, 1),
    vec4(0, 0, 1, 1),
};

layout(location = 0) out vec4 outColor;

void main() 
{
    gl_Position = pos[gl_VertexIndex];
    outColor    = col[gl_VertexIndex];
}