#version 450

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outUV;

void main() 
{
    outUV       = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
    gl_Position = vec4(outUV * 2.0f + -1.0f, 0.0f, 1.0f);
    outColor    = vec4(0, 0, 0, 1);
}