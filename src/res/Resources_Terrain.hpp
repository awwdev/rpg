//https://github.com/awwdev

#pragma once
#include "res/Terrain/Terrain.hpp"

namespace rpg::res {

struct Resources_Terrain
{
    static constexpr auto QUADRANT_COUNT = 2;
    static constexpr auto QUAD_COUNT = 10;
    static constexpr auto QUAD_SIZE  = 1;

    using TERRAIN_T = res2::Terrain<QUADRANT_COUNT, QUAD_COUNT, QUAD_SIZE>;
    TERRAIN_T terrain;

    void Load()
    {
        terrain.Create();
    }
    
};

}//ns