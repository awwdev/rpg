//https://github.com/awwdev

#pragma once
#include "com/Utils.hpp"
#include "com/Matrix.hpp"
//#include "res/Resources_Terrain.hpp"
#include "ecs/ECS.hpp"

namespace rpg::app {

struct CmdTerrainVertexGrab
{
    com::Vec3f position = {};

    void Execute(/*res::Resources_Terrain& resTerrain*/) const
    {
    }

    void Revert(/*res::Resources_Terrain& resTerrain*/) const
    {

    }

};

}//ns