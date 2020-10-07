//https://github.com/awwdev

#pragma once
#include "res/Terrain/Terrain.hpp"

namespace rpg::res {

struct Resources_Terrain
{
    res::Terrain<10, 10, 2> terrain;

    void Load()
    {
        terrain.Create();
    }
};

}//ns