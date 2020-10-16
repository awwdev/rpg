//https://github.com/awwdev

#pragma once
#include "res/Terrain/TerrainGrid.hpp"
#include "gpu/RenderData/RenderData_General.hpp"

namespace rpg::res2 {

struct Quadrant
{   
    //? settings
    static constexpr float QUAD_SIZE  = 1;
    static constexpr idx_t QUAD_COUNT = 10;

    static constexpr idx_t VERT_COUNT = (QUAD_COUNT + 1) * (QUAD_COUNT + 1);
    static constexpr idx_t INDEX_COUNT = VERT_COUNT * 6;
    static constexpr float QUADRANT_SIZE = QUAD_SIZE * QUAD_COUNT;
    using Vertex = gpu::RenderData_General::Vertex;

    //? data
    Vertex   vertices [QUAD_COUNT + 1][QUAD_COUNT + 1];
    uint32_t indices  [INDEX_COUNT];

    void Create(float const index_z, float const index_x)
    {
        constexpr com::Vec4f COLOR { 0.1f, 0.7f, 0.1f, 1 };
        float const offset_z = index_z * QUADRANT_SIZE;
        float const offset_x = index_x * QUADRANT_SIZE;
        CreateGridIndexed(vertices, indices, QUAD_SIZE, QUAD_SIZE, offset_z, offset_x, COLOR);
    }
};

struct Terrain
{
    //? settings
    static constexpr auto QUADRANT_COUNT = 2;

    //? data
    Quadrant quadrants [QUADRANT_COUNT][QUADRANT_COUNT];

    void Create()
    {
        for(auto z = 0; z < QUADRANT_COUNT; ++z) {
        for(auto x = 0; x < QUADRANT_COUNT; ++x) {
            quadrants[z][x].Create(z, x);
        }}
    }

};

} //ns