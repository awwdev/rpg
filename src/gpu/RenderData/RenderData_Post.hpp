
//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/box/Matrix.hpp"

namespace rpg::gpu {

struct RenderData_Post
{
    static constexpr uint32_t VBO_BLUR_MAX = 100; //used for blurring

    struct Vertex
    {
        com::Vec2f pos;
        com::Vec2f tex;
        alignas(2) f32 blur;
    };

    //? DATA
    com::Array<Vertex, VBO_BLUR_MAX> vboBlur;

    void Clear()
    {
        vboBlur.Clear();
    }
};

}//ns