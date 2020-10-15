//https://github.com/awwdev

#pragma once
#include "gpu/RenderData/RenderData_General.hpp"

namespace rpg::res2 {

struct Quadrant
{   
    //? settings
    static constexpr auto QUAD_COUNT = 10;
    static constexpr auto QUAD_SIZE  = 1;
    static constexpr auto VERT_COUNT = QUAD_COUNT * QUAD_COUNT * 6;
    using Vertex = gpu::RenderData_General::Vertex;

    //? data
    Vertex vertices [VERT_COUNT];
};

struct Terrain
{
    //? settings
    static constexpr auto QUADRANT_COUNT = 2;

    //? data
    Quadrant quadrants [QUADRANT_COUNT][QUADRANT_COUNT];
};

//TODO: plane grid mesh creation function (indexed)
//TODO: normal data and color data per glPrimitivID in frag shader

} //ns