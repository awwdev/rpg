//https://github.com/awwdev
#pragma once

#include "res/Terrain/Terrain.hpp"

namespace rpg::res {
    
struct CpuResources
{   
    res::Terrain<10, 10, 2> terrain;
    CpuResources() 
    {
        terrain.Create();
    }
}; 

}//ns