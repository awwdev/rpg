//https://github.com/awwdev

#pragma once
#include "res/Terrain/Terrain.hpp"

namespace rpg::res {

struct Resources_Terrain
{
    static constexpr idx_t QUADRANT_COUNT_ROW = 3;
    static constexpr idx_t QUADRANT_COUNT_TOTAL = QUADRANT_COUNT_ROW * QUADRANT_COUNT_ROW;
    static constexpr idx_t QUAD_COUNT = 10;
    static constexpr idx_t QUAD_SIZE  = 3;

    using TERRAIN_T = res::Terrain<QUADRANT_COUNT_ROW, QUAD_COUNT, QUAD_SIZE>;
    TERRAIN_T terrain;

    void Load()
    {
        terrain.Create();
    }
    
};

}//ns