#version 450

const vec4 tris [] = {
    vec4( 0,-1, 0, 1), 
    vec4( 1, 1, 0, 1),
    vec4(-1, 1, 0, 1),
};

void main() 
{
    gl_Position = tris[gl_VertexIndex];
}