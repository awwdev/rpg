#version 450

const vec4 QUAD [] = {
    vec4(-1, -1,  0, 1),
    vec4( 1, -1,  0, 1),
    vec4( 1,  1,  0, 1),
   
    vec4(-1, -1,  0, 1),
    vec4( 1,  1,  0, 1),
    vec4( 0, -1,  0, 1),
};

const vec2 UV [] = {
    vec2( 0,  0),
    vec2( 1,  0),
    vec2( 1,  1) ,
   
    vec2( 0,  0),
    vec2( 1,  1),
    vec2( 0,  0),
};

layout (location = 0) out vec2 outUV;

void main() 
{
    gl_Position = QUAD[gl_VertexIndex];
    outUV = UV[gl_VertexIndex];
}