//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "com/Utils.hpp"
#include "com/Matrix.hpp"

namespace rpg::res2 {



template<typename VERTEX>
struct GridMesh_Indexed
{
    VERTEX    vertices [2];
    uint32_t  indices  [2];
    com::AABB aabb;

    //triangle lookups
    struct Triangle { uint32_t indices [3]; };
    uint32_t indexToTriangle [2];
    Triangle triangleToIndex [2];

    void Create()
    {

    }

    void Recalculate()
    {

    }
};

/*
template<typename VERTEX, auto COUNT_Z, auto COUNT_X, auto INDEX_COUNT, auto TRIANGLE_COUNT>
void CreateGridIndexed(
    VERTEX (&vertices)[COUNT_Z][COUNT_X], 
    uint32_t (&indices)[INDEX_COUNT],
    Triangle (&triangles)[TRIANGLE_COUNT], 
    float const size_z, float const size_x, 
    float const offset_z, float const offset_x,
    idx_t const offset_v, com::Vec4f const& color)
{
    //vertices sequential 
    for(idx_t z = 0; z < COUNT_Z; ++z) {
    for(idx_t x = 0; x < COUNT_X; ++x) {
        vertices[z][x] = VERTEX {
            .pos = { offset_x + x * size_x, (f32)(rand() % 2), offset_z + z * size_z },
            .nor = { 0, -1, 0 },
            .col = color,
            .tex = {}
        };
    }}

    //indices
    for(idx_t i = 0; i < INDEX_COUNT; i+=6)
    {
        auto v = i / 6;
        auto n = v / (COUNT_X - 1);
        v += n;
        v += offset_v;

        indices[i+0] = v;
        indices[i+1] = v + 1;
        indices[i+2] = v + COUNT_X + 1;
        indices[i+3] = v;
        indices[i+4] = v + COUNT_X + 1;
        indices[i+5] = v + COUNT_X;
    }

    //triangles
    for(idx_t i = 0; i < TRIANGLE_COUNT; i+=2)
    {
        uint32_t const n = i;// + i/(COUNT_X*2 - 1);
        triangles[i+0] = { n, n+6, n+5 };
        triangles[i+1] = { n, n+1, n+6 };
    }
}
*/

}//ns