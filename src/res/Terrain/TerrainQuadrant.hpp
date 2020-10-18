//https://github.com/awwdev

#pragma once
#include "res/Terrain/TerrainGrid.hpp"
#include "gpu/RenderData/RenderData_General.hpp"
#include "com/box/Optional.hpp"

namespace rpg::res2 {

template<auto QUAD_COUNT, auto QUAD_SIZE>
struct Quadrant
{   
    //? meta
    static constexpr idx_t VERTEX_ROW_COUNT = QUAD_COUNT + 1;
    static constexpr idx_t VERTEX_COUNT  = VERTEX_ROW_COUNT * VERTEX_ROW_COUNT;
    static constexpr idx_t INDEX_COUNT   = QUAD_COUNT * QUAD_COUNT * 6;
    static constexpr float QUADRANT_SIZE = QUAD_SIZE * QUAD_COUNT;
    using Vertex = gpu::RenderData_General::Vertex;

    //? data
    Vertex   vertices [VERTEX_ROW_COUNT][VERTEX_ROW_COUNT];
    uint32_t indices  [INDEX_COUNT];
    idx_t quadrantIdx = 0; //assigned by create function

    void Create(float const qIndex_z, float const qIndex_x, idx_t const pQuadrantIdx, com::Vec4f const& color = { 0.1f, 0.7f, 0.1f, 1 })
    {
        quadrantIdx = pQuadrantIdx;
        float const offset_z = qIndex_z * QUADRANT_SIZE;
        float const offset_x = qIndex_x * QUADRANT_SIZE;
        idx_t const offset_v = quadrantIdx * VERTEX_COUNT; //vertex index offset (one holistic vbo)
        CreateGridIndexed(vertices, indices, QUAD_SIZE, QUAD_SIZE, offset_z, offset_x, offset_v, color);
    }

    Vertex& GetVertexByIndex(const auto idx) 
    {
        idx_t const z = idx / VERTEX_ROW_COUNT;
        idx_t const x = idx % VERTEX_ROW_COUNT;
        return vertices[z][x];
    }

};

}//ns