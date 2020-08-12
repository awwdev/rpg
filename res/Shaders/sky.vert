#version 450

layout(push_constant) uniform Push {
    mat4 camera;
    mat4 sun;
    vec4 topColor;
    vec4 botColor;
} push;

layout (location = 0) out vec4 outCol;

const float S = 200;
const float H =  20;
const float Y = -10;
const vec4 skybox [] = {
    vec4(-S, -( H) + Y,  S,  1),   // Front-top-left
    vec4( S, -( H) + Y,  S,  1),   // Front-top-right
    vec4(-S, -(-H) + Y,  S,  1),   // Front-bottom-left
    vec4( S, -(-H) + Y,  S,  1),   // Front-bottom-right
    vec4( S, -(-H) + Y, -S,  1),   // Back-bottom-right
    vec4( S, -( H) + Y,  S,  1),   // Front-top-right
    vec4( S, -( H) + Y, -S,  1),   // Back-top-right
    vec4(-S, -( H) + Y,  S,  1),   // Front-top-left
    vec4(-S, -( H) + Y, -S,  1),   // Back-top-left
    vec4(-S, -(-H) + Y,  S,  1),   // Front-bottom-left
    vec4(-S, -(-H) + Y, -S,  1),   // Back-bottom-left
    vec4( S, -(-H) + Y, -S,  1),   // Back-bottom-right
    vec4(-S, -( H) + Y, -S,  1),   // Back-top-left
    vec4( S, -( H) + Y, -S,  1),   // Back-top-right
};

const vec2 colors [] = {
    vec2(1, 0),   // Front-top-left
    vec2(1, 0),   // Front-top-right
    vec2(0, 1),   // Front-bottom-left
    vec2(0, 1),   // Front-bottom-right
    vec2(0, 1),   // Back-bottom-right
    vec2(1, 0),   // Front-top-right
    vec2(1, 0),   // Back-top-right
    vec2(1, 0),   // Front-top-left
    vec2(1, 0),   // Back-top-left
    vec2(0, 1),   // Front-bottom-left
    vec2(0, 1),   // Back-bottom-left
    vec2(0, 1),   // Back-bottom-right
    vec2(1, 0),   // Back-top-left
    vec2(1, 0),   // Back-top-right
};

void main() 
{
    gl_Position = push.camera * skybox[gl_VertexIndex];
    outCol      = colors[gl_VertexIndex].x * push.topColor + colors[gl_VertexIndex].y * push.botColor;
}