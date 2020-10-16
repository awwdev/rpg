//https://github.com/awwdev

#pragma once
#include "res/Terrain/Terrain.hpp"

namespace rpg::res {

struct Resources_Terrain
{
    res2::Terrain terrain;

    void Load()
    {
        terrain.Create();
    }
    
};

}//ns