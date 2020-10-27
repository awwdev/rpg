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
    com::Vec3f placementPoint;
    ecs::ID prefabID;

    EditorCmd_PrefabPlacement(res::PrefabEnum const pPrefabEnum, com::Vec3f const& pPoint)
        : prefabEnum { pPrefabEnum }
        , placementPoint { pPoint }
    {}

    /*
    void Execute(res::Resources&, ecs::ECS& ecs)
    {   
        prefabID = ecs.AddEntity(prefabEnum);
        auto& mainComponent = ecs.arrays.mainComponents.Get(prefabID);
        mainComponent.translation = placementPoint;
    }
    
    void ExecuteReverse(res::Resources&, ecs::ECS& ecs)
    {
        ecs.RemoveEntity(prefabID);
    }
    */

    template<EditorCommandDirection DIR>
    void Execute(res::Resources& res, ecs::ECS& ecs)
    {
    }
};

} //ns