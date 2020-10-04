//https://github.com/awwdev
#pragma once

#include "com/box/Array.hpp"
#include "com/box/EnumMap.hpp"
#include "dbg/Logger.hpp"
#include "res/_Old/TextureLoader.hpp"
#include "res/_Old/TextureArray.hpp"
#include "res/_Old/CpuResources.hpp"
#include "res/Terrain/Terrain.hpp"
#include "gpu/RenderData/RenderData.hpp"

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