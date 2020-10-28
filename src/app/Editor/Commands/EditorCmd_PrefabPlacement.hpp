//https://github.com/awwdev

#pragma once
#include "ecs/ECS.hpp"
#include "res/Resources.hpp"
#include "res/Prefab/PrefabEnum.hpp"
#include "app/Editor/EditorCommandDirection.hpp"

namespace rpg::app {

struct EditorCmd_PrefabPlacement
{
    res::PrefabEnum prefabEnum;
    com::Vec3f position;
    ecs::ID prefabID;

    template<EditorCommandDirection DIR>
    void Execute(res::Resources& res, ecs::ECS& ecs)
    {
        if constexpr(DIR == EditorCommandDirection::Backwards)
        {
            ecs.RemoveEntity(prefabID);
        }
        if constexpr(DIR == EditorCommandDirection::Forwards)
        {
            prefabID = ecs.AddEntity(prefabEnum);
            auto& mainComponent = ecs.arrays.mainComponents.Get(prefabID);
            mainComponent.translation = position;
        }
    }

};

} //ns