//https://github.com/awwdev

#pragma once
#include "com/Utils.hpp"
#include "com/Matrix.hpp"
#include "res/Prefab/PrefabEnum.hpp"
#include "ecs/ECS.hpp"

namespace rpg::app {

struct CmdCreateEntityFromPrefab
{
    res::PrefabEnum prefabEnum  = res::PrefabEnum::Cube;
    com::Vec3f position         = {};
    com::Vec3f rotation         = {};
    com::Vec3f scale            = { 1, 1, 1 };

    ecs::ID entityID;

    void Execute(ecs::ECS& ecs)
    {
        entityID = ecs.AddEntity(prefabEnum);
        auto& mainComponent = ecs.arrays.mainComponents.Get(entityID);
        mainComponent.translation = position;
        mainComponent.rotation = rotation;
        mainComponent.scale = scale;
    }

    void ExecuteReverse(ecs::ECS& ecs) const
    {
        ecs.RemoveEntity(entityID);
    }
};

}//ns