//https://github.com/awwdev

#pragma once
#include "com/Utils.hpp"
#include "com/Matrix.hpp"

namespace rpg::res2 {

template<typename VERTEX, auto COUNT_Z, auto COUNT_X>
void CreateGridIndexed(
    VERTEX   (&vertices)[COUNT_Z][COUNT_X], 
    uint32_t (&indices) [COUNT_Z * COUNT_X * 6],
    float const size_z,   float const size_x, 
    float const offset_z, float const offset_x,
    com::Vec4f const& color)
{
    //vertices sequential 
    for(idx_t z = 0; z < COUNT_Z + 1; ++z) {
    for(idx_t x = 0; x < COUNT_X + 1; ++x) {
        vertices[z][x] = VERTEX {
            .pos = { offset_x + x * size_x, 0, offset_z + z * size_z },
            .nor = { 0, -1, 0 },
            .col = color,
            .tex = {}
        };
    }}

    //indices
    for(idx_t i = 0; i < COUNT_Z * COUNT_X * 6; i+=6)
    {
        indices[i+0] = i;
        indices[i+1] = i + 1;
        indices[i+2] = i + COUNT_X + 1;
        indices[i+3] = i;
        indices[i+4] = i + COUNT_X + 1;;
        indices[i+5] = i + COUNT_X;
    }
}

}//ns