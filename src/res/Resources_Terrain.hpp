//https://github.com/awwdev

#pragma once
#include "res/Terrain/Terrain.hpp"

namespace rpg::res {

struct Resources_Terrain
{
    static constexpr auto QUADRANT_COUNT_ROW = 2;
    static constexpr auto QUADRANT_COUNT_TOTAL = QUADRANT_COUNT_ROW * QUADRANT_COUNT_ROW;
    static constexpr auto QUAD_COUNT = 10;
    static constexpr auto QUAD_SIZE  = 3;

    using TERRAIN_T = res::Terrain<QUADRANT_COUNT_ROW, QUAD_COUNT, QUAD_SIZE>;
    TERRAIN_T terrain;

    void Load()
    {
        terrain.Create();
    }
    
};

}//ns