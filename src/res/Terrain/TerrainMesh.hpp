//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "com/utils/Utils.hpp"
#include "com/Matrix.hpp"

namespace rpg::res2 {

template<typename VERTEX_T, auto QUAD_COUNT_T>
struct TerrainMeshIndexed
{
    //? meta
    static constexpr idx_t QUAD_COUNT = QUAD_COUNT_T;
    static constexpr idx_t VERTEX_COUNT_ROW = QUAD_COUNT + 1;
    static constexpr idx_t VERTEX_COUNT_TOTAL = VERTEX_COUNT_ROW * VERTEX_COUNT_ROW;
    static constexpr idx_t INDEX_COUNT = QUAD_COUNT * QUAD_COUNT * 6;
    static constexpr idx_t TRIANGLE_COUNT = QUAD_COUNT * QUAD_COUNT * 2;
    using Vertex = VERTEX_T;

    //? data
    Vertex    vertices [VERTEX_COUNT_TOTAL];
    uint32_t  absoluteIndices [INDEX_COUNT];
    uint32_t  indicesOffset;
    com::AABB aabb;

    auto GetRelativeVertexIndex(auto const idx) const { return absoluteIndices[idx] - indicesOffset; }

    void Create(
        float const size_z, float const size_x, 
        float const offset_z, float const offset_x,
        idx_t const offset_indices, com::Vec4f const& color)
    {
        //vertices
        FOR_C_ARRAY(vertices, i) 
        {
            const auto x = i % VERTEX_COUNT_ROW;
            const auto y = (f32)(rand() % 10) / 20.f; //testing aabb
            const auto z = i / VERTEX_COUNT_ROW;
            vertices[i] = 
            {
                .pos = { offset_x + x * size_x, y, offset_z + z * size_z },
                .nor = { 0, -1, 0 },
                .col = color,
            };
        }

        //indices
        indicesOffset = offset_indices;
        for(idx_t i = 0; i < INDEX_COUNT; i+=6)
        {
            auto v = i / 6;
            auto n = v / (VERTEX_COUNT_ROW - 1);
            v += n;
            v += indicesOffset;

            absoluteIndices[i+0] = v;
            absoluteIndices[i+1] = v + VERTEX_COUNT_ROW + 1;
            absoluteIndices[i+2] = v + 1;
            absoluteIndices[i+3] = v;
            absoluteIndices[i+4] = v + VERTEX_COUNT_ROW;
            absoluteIndices[i+5] = v + VERTEX_COUNT_ROW + 1;
        }

        Recalculate();
    }

    void Recalculate()
    {
        aabb = com::CalculateAABB(vertices, VERTEX_COUNT_TOTAL);
        //normals
    }

};

}//ns