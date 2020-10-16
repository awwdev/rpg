//https://github.com/awwdev

#pragma once
#include "res/Terrain/TerrainGrid.hpp"
#include "gpu/RenderData/RenderData_General.hpp"

namespace rpg::res2 {

template<auto QUAD_COUNT, auto QUAD_SIZE>
struct Quadrant
{   
    //? meta
    static constexpr idx_t VERTEX_COUNT = (QUAD_COUNT + 1) * (QUAD_COUNT + 1);
    static constexpr idx_t INDEX_COUNT  = QUAD_COUNT * QUAD_COUNT * 6;
    static constexpr float QUADRANT_SIZE = QUAD_SIZE * QUAD_COUNT;
    using Vertex = gpu::RenderData_General::Vertex;

    //? data
    Vertex   vertices [QUAD_COUNT + 1][QUAD_COUNT + 1];
    uint32_t indices  [INDEX_COUNT];

    void Create(float const index_z, float const index_x, idx_t const quadrantIdx,
    com::Vec4f const& color = { 0.1f, 0.7f, 0.1f, 1 })
    {
        float const offset_z = index_z * QUADRANT_SIZE;
        float const offset_x = index_x * QUADRANT_SIZE;
        idx_t const offset_v = quadrantIdx * VERTEX_COUNT;
        CreateGridIndexed(vertices, indices, QUAD_SIZE, QUAD_SIZE, offset_z, offset_x, offset_v, color);
    }
};

}//ns