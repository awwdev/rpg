//https://github.com/awwdev

#pragma once
#include "app/Editor/Commands/EditorCmd_Base.hpp"

namespace rpg::app {

struct EditorCmd_PrefabPlacement : EditorCmd_Base
{
    void Execute(res::Resources&, ecs::ECS&) override
    {

    }
    
    void ExecuteReverse(res::Resources&, ecs::ECS&) override
    {

    }
};

} //ns