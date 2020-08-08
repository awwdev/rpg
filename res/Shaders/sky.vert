#version 450

layout(push_constant) uniform Push {
    mat4 camera;
    mat4 sun;
    vec3 sunDir;
} push;

layout (location = 0) out vec4 outCol;

const mat4 scale = mat4(
    100, 0, 0, 0,
    0, 100, 0, 0,
    0, 0, 100, 0,
    0, 30, 0, 1
);

const vec4 sphere [] = {
    vec4(-0.000000,-(-0.382684), 0.923879, 1),
    vec4( 0.707107,-(-0.000000), 0.707107, 1),
    vec4( 0.653281,-(-0.382684), 0.653282, 1),
    vec4(-0.000000,-( 0.382683), 0.923879, 1),
    vec4( 0.500000,-( 0.707107), 0.500000, 1),
    vec4( 0.653281,-( 0.382683), 0.653282, 1),
    vec4(-0.000000,-( 0.923880), 0.382683, 1),
    vec4(-0.000000,-( 1.000000), 0.000000, 1),
    vec4( 0.270598,-( 0.923880), 0.270598, 1),
    vec4( 0.000000,-(-1.000000), 0.000000, 1),
    vec4(-0.000000,-(-0.923880), 0.382683, 1),
    vec4( 0.270598,-(-0.923880), 0.270598, 1),
    vec4(-0.000000,-(-0.382684), 0.923879, 1),
    vec4( 0.500000,-(-0.707107), 0.500000, 1),
    vec4(-0.000000,-(-0.707107), 0.707107, 1),
    vec4(-0.000000,-( 0.382683), 0.923879, 1),
    vec4( 0.707107,-(-0.000000), 0.707107, 1),
    vec4( 0.000000,-(-0.000000), 1.000000, 1),
    vec4(-0.000000,-( 0.707107), 0.707107, 1),
    vec4( 0.270598,-( 0.923880), 0.270598, 1),
    vec4( 0.500000,-( 0.707107), 0.500000, 1),
    vec4(-0.000000,-(-0.707107), 0.707107, 1),
    vec4( 0.270598,-(-0.923880), 0.270598, 1),
    vec4(-0.000000,-(-0.923880), 0.382683, 1),
    vec4( 0.000000,-(-1.000000), 0.000000, 1),
    vec4( 0.270598,-(-0.923880), 0.270598, 1),
    vec4( 0.382683,-(-0.923880), 0.000000, 1),
    vec4( 0.653281,-(-0.382684), 0.653282, 1),
    vec4( 0.707107,-(-0.707107), 0.000000, 1),
    vec4( 0.500000,-(-0.707107), 0.500000, 1),
    vec4( 0.653281,-( 0.382683), 0.653282, 1),
    vec4( 1.000000,-(-0.000000), 0.000000, 1),
    vec4( 0.707107,-(-0.000000), 0.707107, 1),
    vec4( 0.270598,-( 0.923880), 0.270598, 1),
    vec4( 0.707107,-( 0.707107), 0.000000, 1),
    vec4( 0.500000,-( 0.707107), 0.500000, 1),
    vec4( 0.270598,-(-0.923880), 0.270598, 1),
    vec4( 0.707107,-(-0.707107), 0.000000, 1),
    vec4( 0.382683,-(-0.923880), 0.000000, 1),
    vec4( 0.707107,-(-0.000000), 0.707107, 1),
    vec4( 0.923879,-(-0.382684), 0.000000, 1),
    vec4( 0.653281,-(-0.382684), 0.653282, 1),
    vec4( 0.500000,-( 0.707107), 0.500000, 1),
    vec4( 0.923879,-( 0.382683), 0.000000, 1),
    vec4( 0.653281,-( 0.382683), 0.653282, 1),
    vec4( 0.270598,-( 0.923880), 0.270598, 1),
    vec4(-0.000000,-( 1.000000), 0.000000, 1),
    vec4( 0.382683,-( 0.923880), 0.000000, 1),
    vec4( 0.707107,-( 0.707107), 0.000000, 1),
    vec4( 0.270598,-( 0.923880),-0.270598, 1),
    vec4( 0.500000,-( 0.707107),-0.500000, 1),
    vec4( 0.707107,-(-0.707107), 0.000000, 1),
    vec4( 0.270598,-(-0.923880),-0.270598, 1),
    vec4( 0.382683,-(-0.923880), 0.000000, 1),
    vec4( 1.000000,-(-0.000000), 0.000000, 1),
    vec4( 0.653281,-(-0.382684),-0.653281, 1),
    vec4( 0.923879,-(-0.382684), 0.000000, 1),
    vec4( 0.707107,-( 0.707107), 0.000000, 1),
    vec4( 0.653281,-( 0.382683),-0.653281, 1),
    vec4( 0.923879,-( 0.382683), 0.000000, 1),
    vec4( 0.382683,-( 0.923880), 0.000000, 1),
    vec4(-0.000000,-( 1.000000), 0.000000, 1),
    vec4( 0.270598,-( 0.923880),-0.270598, 1),
    vec4( 0.000000,-(-1.000000), 0.000000, 1),
    vec4( 0.382683,-(-0.923880), 0.000000, 1),
    vec4( 0.270598,-(-0.923880),-0.270598, 1),
    vec4( 0.923879,-(-0.382684), 0.000000, 1),
    vec4( 0.500000,-(-0.707107),-0.500000, 1),
    vec4( 0.707107,-(-0.707107), 0.000000, 1),
    vec4( 0.923879,-( 0.382683), 0.000000, 1),
    vec4( 0.707107,-(-0.000000),-0.707106, 1),
    vec4( 1.000000,-(-0.000000), 0.000000, 1),
    vec4( 0.500000,-(-0.707107),-0.500000, 1),
    vec4( 0.000000,-(-0.923880),-0.382683, 1),
    vec4( 0.270598,-(-0.923880),-0.270598, 1),
    vec4( 0.707107,-(-0.000000),-0.707106, 1),
    vec4( 0.000000,-(-0.382684),-0.923879, 1),
    vec4( 0.653281,-(-0.382684),-0.653281, 1),
    vec4( 0.500000,-( 0.707107),-0.500000, 1),
    vec4( 0.000000,-( 0.382683),-0.923879, 1),
    vec4( 0.653281,-( 0.382683),-0.653281, 1),
    vec4( 0.270598,-( 0.923880),-0.270598, 1),
    vec4(-0.000000,-( 1.000000), 0.000000, 1),
    vec4( 0.000000,-( 0.923880),-0.382683, 1),
    vec4( 0.000000,-(-1.000000), 0.000000, 1),
    vec4( 0.270598,-(-0.923880),-0.270598, 1),
    vec4( 0.000000,-(-0.923880),-0.382683, 1),
    vec4( 0.500000,-(-0.707107),-0.500000, 1),
    vec4( 0.000000,-(-0.382684),-0.923879, 1),
    vec4( 0.000000,-(-0.707107),-0.707106, 1),
    vec4( 0.653281,-( 0.382683),-0.653281, 1),
    vec4( 0.000000,-(-0.000000),-1.000000, 1),
    vec4( 0.707107,-(-0.000000),-0.707106, 1),
    vec4( 0.270598,-( 0.923880),-0.270598, 1),
    vec4( 0.000000,-( 0.707107),-0.707106, 1),
    vec4( 0.500000,-( 0.707107),-0.500000, 1),
    vec4( 0.000000,-( 0.923880),-0.382683, 1),
    vec4(-0.000000,-( 1.000000), 0.000000, 1),
    vec4(-0.270598,-( 0.923880),-0.270598, 1),
    vec4( 0.000000,-(-1.000000), 0.000000, 1),
    vec4( 0.000000,-(-0.923880),-0.382683, 1),
    vec4(-0.270598,-(-0.923880),-0.270598, 1),
    vec4( 0.000000,-(-0.707107),-0.707106, 1),
    vec4(-0.653281,-(-0.382684),-0.653281, 1),
    vec4(-0.500000,-(-0.707107),-0.500000, 1),
    vec4( 0.000000,-(-0.000000),-1.000000, 1),
    vec4(-0.653281,-( 0.382683),-0.653281, 1),
    vec4(-0.707107,-(-0.000000),-0.707106, 1),
    vec4( 0.000000,-( 0.707107),-0.707106, 1),
    vec4(-0.270598,-( 0.923880),-0.270598, 1),
    vec4(-0.500000,-( 0.707107),-0.500000, 1),
    vec4( 0.000000,-(-0.707107),-0.707106, 1),
    vec4(-0.270598,-(-0.923880),-0.270598, 1),
    vec4( 0.000000,-(-0.923880),-0.382683, 1),
    vec4( 0.000000,-(-0.000000),-1.000000, 1),
    vec4(-0.653281,-(-0.382684),-0.653281, 1),
    vec4( 0.000000,-(-0.382684),-0.923879, 1),
    vec4( 0.000000,-( 0.707107),-0.707106, 1),
    vec4(-0.653281,-( 0.382683),-0.653281, 1),
    vec4( 0.000000,-( 0.382683),-0.923879, 1),
    vec4(-0.653281,-(-0.382684),-0.653281, 1),
    vec4(-0.707107,-(-0.707107), 0.000000, 1),
    vec4(-0.500000,-(-0.707107),-0.500000, 1),
    vec4(-0.707107,-(-0.000000),-0.707106, 1),
    vec4(-0.923879,-( 0.382683), 0.000000, 1),
    vec4(-1.000000,-(-0.000000), 0.000000, 1),
    vec4(-0.270598,-( 0.923880),-0.270598, 1),
    vec4(-0.707107,-( 0.707107), 0.000000, 1),
    vec4(-0.500000,-( 0.707107),-0.500000, 1),
    vec4(-0.500000,-(-0.707107),-0.500000, 1),
    vec4(-0.382683,-(-0.923880), 0.000000, 1),
    vec4(-0.270598,-(-0.923880),-0.270598, 1),
    vec4(-0.707107,-(-0.000000),-0.707106, 1),
    vec4(-0.923879,-(-0.382684), 0.000000, 1),
    vec4(-0.653281,-(-0.382684),-0.653281, 1),
    vec4(-0.500000,-( 0.707107),-0.500000, 1),
    vec4(-0.923879,-( 0.382683), 0.000000, 1),
    vec4(-0.653281,-( 0.382683),-0.653281, 1),
    vec4(-0.270598,-( 0.923880),-0.270598, 1),
    vec4(-0.000000,-( 1.000000), 0.000000, 1),
    vec4(-0.382683,-( 0.923880), 0.000000, 1),
    vec4( 0.000000,-(-1.000000), 0.000000, 1),
    vec4(-0.270598,-(-0.923880),-0.270598, 1),
    vec4(-0.382683,-(-0.923880), 0.000000, 1),
    vec4(-0.382683,-(-0.923880), 0.000000, 1),
    vec4(-0.500000,-(-0.707107), 0.500000, 1),
    vec4(-0.270598,-(-0.923880), 0.270598, 1),
    vec4(-1.000000,-(-0.000000), 0.000000, 1),
    vec4(-0.653281,-(-0.382684), 0.653281, 1),
    vec4(-0.923879,-(-0.382684), 0.000000, 1),
    vec4(-0.707107,-( 0.707107), 0.000000, 1),
    vec4(-0.653281,-( 0.382683), 0.653281, 1),
    vec4(-0.923879,-( 0.382683), 0.000000, 1),
    vec4(-0.382683,-( 0.923880), 0.000000, 1),
    vec4(-0.000000,-( 1.000000), 0.000000, 1),
    vec4(-0.270598,-( 0.923880), 0.270598, 1),
    vec4( 0.000000,-(-1.000000), 0.000000, 1),
    vec4(-0.382683,-(-0.923880), 0.000000, 1),
    vec4(-0.270598,-(-0.923880), 0.270598, 1),
    vec4(-0.707107,-(-0.707107), 0.000000, 1),
    vec4(-0.653281,-(-0.382684), 0.653281, 1),
    vec4(-0.500000,-(-0.707107), 0.500000, 1),
    vec4(-1.000000,-(-0.000000), 0.000000, 1),
    vec4(-0.653281,-( 0.382683), 0.653281, 1),
    vec4(-0.707107,-(-0.000000), 0.707107, 1),
    vec4(-0.382683,-( 0.923880), 0.000000, 1),
    vec4(-0.500000,-( 0.707107), 0.500000, 1),
    vec4(-0.707107,-( 0.707107), 0.000000, 1),
    vec4(-0.707107,-(-0.000000), 0.707107, 1),
    vec4(-0.000000,-(-0.382684), 0.923879, 1),
    vec4(-0.653281,-(-0.382684), 0.653281, 1),
    vec4(-0.500000,-( 0.707107), 0.500000, 1),
    vec4(-0.000000,-( 0.382683), 0.923879, 1),
    vec4(-0.653281,-( 0.382683), 0.653281, 1),
    vec4(-0.270598,-( 0.923880), 0.270598, 1),
    vec4(-0.000000,-( 1.000000), 0.000000, 1),
    vec4(-0.000000,-( 0.923880), 0.382683, 1),
    vec4( 0.000000,-(-1.000000), 0.000000, 1),
    vec4(-0.270598,-(-0.923880), 0.270598, 1),
    vec4(-0.000000,-(-0.923880), 0.382683, 1),
    vec4(-0.653281,-(-0.382684), 0.653281, 1),
    vec4(-0.000000,-(-0.707107), 0.707107, 1),
    vec4(-0.500000,-(-0.707107), 0.500000, 1),
    vec4(-0.653281,-( 0.382683), 0.653281, 1),
    vec4( 0.000000,-(-0.000000), 1.000000, 1),
    vec4(-0.707107,-(-0.000000), 0.707107, 1),
    vec4(-0.270598,-( 0.923880), 0.270598, 1),
    vec4(-0.000000,-( 0.707107), 0.707107, 1),
    vec4(-0.500000,-( 0.707107), 0.500000, 1),
    vec4(-0.500000,-(-0.707107), 0.500000, 1),
    vec4(-0.000000,-(-0.923880), 0.382683, 1),
    vec4(-0.270598,-(-0.923880), 0.270598, 1),
    vec4(-0.000000,-(-0.382684), 0.923879, 1),
    vec4( 0.000000,-(-0.000000), 1.000000, 1),
    vec4( 0.707107,-(-0.000000), 0.707107, 1),
    vec4(-0.000000,-( 0.382683), 0.923879, 1),
    vec4(-0.000000,-( 0.707107), 0.707107, 1),
    vec4( 0.500000,-( 0.707107), 0.500000, 1),
    vec4(-0.000000,-(-0.382684), 0.923879, 1),
    vec4( 0.653281,-(-0.382684), 0.653282, 1),
    vec4( 0.500000,-(-0.707107), 0.500000, 1),
    vec4(-0.000000,-( 0.382683), 0.923879, 1),
    vec4( 0.653281,-( 0.382683), 0.653282, 1),
    vec4( 0.707107,-(-0.000000), 0.707107, 1),
    vec4(-0.000000,-( 0.707107), 0.707107, 1),
    vec4(-0.000000,-( 0.923880), 0.382683, 1),
    vec4( 0.270598,-( 0.923880), 0.270598, 1),
    vec4(-0.000000,-(-0.707107), 0.707107, 1),
    vec4( 0.500000,-(-0.707107), 0.500000, 1),
    vec4( 0.270598,-(-0.923880), 0.270598, 1),
    vec4( 0.653281,-(-0.382684), 0.653282, 1),
    vec4( 0.923879,-(-0.382684), 0.000000, 1),
    vec4( 0.707107,-(-0.707107), 0.000000, 1),
    vec4( 0.653281,-( 0.382683), 0.653282, 1),
    vec4( 0.923879,-( 0.382683), 0.000000, 1),
    vec4( 1.000000,-(-0.000000), 0.000000, 1),
    vec4( 0.270598,-( 0.923880), 0.270598, 1),
    vec4( 0.382683,-( 0.923880), 0.000000, 1),
    vec4( 0.707107,-( 0.707107), 0.000000, 1),
    vec4( 0.270598,-(-0.923880), 0.270598, 1),
    vec4( 0.500000,-(-0.707107), 0.500000, 1),
    vec4( 0.707107,-(-0.707107), 0.000000, 1),
    vec4( 0.707107,-(-0.000000), 0.707107, 1),
    vec4( 1.000000,-(-0.000000), 0.000000, 1),
    vec4( 0.923879,-(-0.382684), 0.000000, 1),
    vec4( 0.500000,-( 0.707107), 0.500000, 1),
    vec4( 0.707107,-( 0.707107), 0.000000, 1),
    vec4( 0.923879,-( 0.382683), 0.000000, 1),
    vec4( 0.707107,-( 0.707107), 0.000000, 1),
    vec4( 0.382683,-( 0.923880), 0.000000, 1),
    vec4( 0.270598,-( 0.923880),-0.270598, 1),
    vec4( 0.707107,-(-0.707107), 0.000000, 1),
    vec4( 0.500000,-(-0.707107),-0.500000, 1),
    vec4( 0.270598,-(-0.923880),-0.270598, 1),
    vec4( 1.000000,-(-0.000000), 0.000000, 1),
    vec4( 0.707107,-(-0.000000),-0.707106, 1),
    vec4( 0.653281,-(-0.382684),-0.653281, 1),
    vec4( 0.707107,-( 0.707107), 0.000000, 1),
    vec4( 0.500000,-( 0.707107),-0.500000, 1),
    vec4( 0.653281,-( 0.382683),-0.653281, 1),
    vec4( 0.923879,-(-0.382684), 0.000000, 1),
    vec4( 0.653281,-(-0.382684),-0.653281, 1),
    vec4( 0.500000,-(-0.707107),-0.500000, 1),
    vec4( 0.923879,-( 0.382683), 0.000000, 1),
    vec4( 0.653281,-( 0.382683),-0.653281, 1),
    vec4( 0.707107,-(-0.000000),-0.707106, 1),
    vec4( 0.500000,-(-0.707107),-0.500000, 1),
    vec4( 0.000000,-(-0.707107),-0.707106, 1),
    vec4( 0.000000,-(-0.923880),-0.382683, 1),
    vec4( 0.707107,-(-0.000000),-0.707106, 1),
    vec4( 0.000000,-(-0.000000),-1.000000, 1),
    vec4( 0.000000,-(-0.382684),-0.923879, 1),
    vec4( 0.500000,-( 0.707107),-0.500000, 1),
    vec4( 0.000000,-( 0.707107),-0.707106, 1),
    vec4( 0.000000,-( 0.382683),-0.923879, 1),
    vec4( 0.500000,-(-0.707107),-0.500000, 1),
    vec4( 0.653281,-(-0.382684),-0.653281, 1),
    vec4( 0.000000,-(-0.382684),-0.923879, 1),
    vec4( 0.653281,-( 0.382683),-0.653281, 1),
    vec4( 0.000000,-( 0.382683),-0.923879, 1),
    vec4( 0.000000,-(-0.000000),-1.000000, 1),
    vec4( 0.270598,-( 0.923880),-0.270598, 1),
    vec4( 0.000000,-( 0.923880),-0.382683, 1),
    vec4( 0.000000,-( 0.707107),-0.707106, 1),
    vec4( 0.000000,-(-0.707107),-0.707106, 1),
    vec4( 0.000000,-(-0.382684),-0.923879, 1),
    vec4(-0.653281,-(-0.382684),-0.653281, 1),
    vec4( 0.000000,-(-0.000000),-1.000000, 1),
    vec4( 0.000000,-( 0.382683),-0.923879, 1),
    vec4(-0.653281,-( 0.382683),-0.653281, 1),
    vec4( 0.000000,-( 0.707107),-0.707106, 1),
    vec4( 0.000000,-( 0.923880),-0.382683, 1),
    vec4(-0.270598,-( 0.923880),-0.270598, 1),
    vec4( 0.000000,-(-0.707107),-0.707106, 1),
    vec4(-0.500000,-(-0.707107),-0.500000, 1),
    vec4(-0.270598,-(-0.923880),-0.270598, 1),
    vec4( 0.000000,-(-0.000000),-1.000000, 1),
    vec4(-0.707107,-(-0.000000),-0.707106, 1),
    vec4(-0.653281,-(-0.382684),-0.653281, 1),
    vec4( 0.000000,-( 0.707107),-0.707106, 1),
    vec4(-0.500000,-( 0.707107),-0.500000, 1),
    vec4(-0.653281,-( 0.382683),-0.653281, 1),
    vec4(-0.653281,-(-0.382684),-0.653281, 1),
    vec4(-0.923879,-(-0.382684), 0.000000, 1),
    vec4(-0.707107,-(-0.707107), 0.000000, 1),
    vec4(-0.707107,-(-0.000000),-0.707106, 1),
    vec4(-0.653281,-( 0.382683),-0.653281, 1),
    vec4(-0.923879,-( 0.382683), 0.000000, 1),
    vec4(-0.270598,-( 0.923880),-0.270598, 1),
    vec4(-0.382683,-( 0.923880), 0.000000, 1),
    vec4(-0.707107,-( 0.707107), 0.000000, 1),
    vec4(-0.500000,-(-0.707107),-0.500000, 1),
    vec4(-0.707107,-(-0.707107), 0.000000, 1),
    vec4(-0.382683,-(-0.923880), 0.000000, 1),
    vec4(-0.707107,-(-0.000000),-0.707106, 1),
    vec4(-1.000000,-(-0.000000), 0.000000, 1),
    vec4(-0.923879,-(-0.382684), 0.000000, 1),
    vec4(-0.500000,-( 0.707107),-0.500000, 1),
    vec4(-0.707107,-( 0.707107), 0.000000, 1),
    vec4(-0.923879,-( 0.382683), 0.000000, 1),
    vec4(-0.382683,-(-0.923880), 0.000000, 1),
    vec4(-0.707107,-(-0.707107), 0.000000, 1),
    vec4(-0.500000,-(-0.707107), 0.500000, 1),
    vec4(-1.000000,-(-0.000000), 0.000000, 1),
    vec4(-0.707107,-(-0.000000), 0.707107, 1),
    vec4(-0.653281,-(-0.382684), 0.653281, 1),
    vec4(-0.707107,-( 0.707107), 0.000000, 1),
    vec4(-0.500000,-( 0.707107), 0.500000, 1),
    vec4(-0.653281,-( 0.382683), 0.653281, 1),
    vec4(-0.707107,-(-0.707107), 0.000000, 1),
    vec4(-0.923879,-(-0.382684), 0.000000, 1),
    vec4(-0.653281,-(-0.382684), 0.653281, 1),
    vec4(-1.000000,-(-0.000000), 0.000000, 1),
    vec4(-0.923879,-( 0.382683), 0.000000, 1),
    vec4(-0.653281,-( 0.382683), 0.653281, 1),
    vec4(-0.382683,-( 0.923880), 0.000000, 1),
    vec4(-0.270598,-( 0.923880), 0.270598, 1),
    vec4(-0.500000,-( 0.707107), 0.500000, 1),
    vec4(-0.707107,-(-0.000000), 0.707107, 1),
    vec4( 0.000000,-(-0.000000), 1.000000, 1),
    vec4(-0.000000,-(-0.382684), 0.923879, 1),
    vec4(-0.500000,-( 0.707107), 0.500000, 1),
    vec4(-0.000000,-( 0.707107), 0.707107, 1),
    vec4(-0.000000,-( 0.382683), 0.923879, 1),
    vec4(-0.653281,-(-0.382684), 0.653281, 1),
    vec4(-0.000000,-(-0.382684), 0.923879, 1),
    vec4(-0.000000,-(-0.707107), 0.707107, 1),
    vec4(-0.653281,-( 0.382683), 0.653281, 1),
    vec4(-0.000000,-( 0.382683), 0.923879, 1),
    vec4( 0.000000,-(-0.000000), 1.000000, 1),
    vec4(-0.270598,-( 0.923880), 0.270598, 1),
    vec4(-0.000000,-( 0.923880), 0.382683, 1),
    vec4(-0.000000,-( 0.707107), 0.707107, 1),
    vec4(-0.500000,-(-0.707107), 0.500000, 1),
    vec4(-0.000000,-(-0.707107), 0.707107, 1),
    vec4(-0.000000,-(-0.923880), 0.382683, 1),
};

const vec4 colors[] = 
{
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.423529,0.803922,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
    vec4(0.909804,0.960784,1.000000, 1),
};

void main() 
{
    gl_Position = push.camera * scale * sphere[gl_VertexIndex];
    outCol      = colors[gl_VertexIndex];
}