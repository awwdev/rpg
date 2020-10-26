//https://github.com/awwdev

#pragma once
#include "ecs/ECS.hpp"
#include "res/Resources.hpp"

namespace rpg::app {

struct EditorCmd_TerrainVertexMove
{
    void Execute(res::Resources&, ecs::ECS&)
    {

    }
    
    void ExecuteReverse(res::Resources&, ecs::ECS&)
    {

    }
};

} //ns