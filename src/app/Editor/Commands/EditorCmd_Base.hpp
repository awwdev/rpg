//https://github.com/awwdev

#pragma once
#include "ecs/ECS.hpp"
#include "res/Resources.hpp"

namespace rpg::app {

struct EditorCmd_Base
{
    virtual void Execute(res::Resources&, ecs::ECS&) = 0;
    virtual void ExecuteReverse(res::Resources&, ecs::ECS&) = 0;
};

} //ns