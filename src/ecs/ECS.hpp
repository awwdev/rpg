//https://github.com/awwdev

#pragma once
#include "com/box/Bitset.hpp"
#include "res/Prefab/PrefabEnum.hpp"

#include "ecs/ComponentsMeta/ComponentArrays.hpp"
#include "gpu/RenderData/RenderData.hpp"

#include "ecs/Systems/RenderSystem.hpp"
#include "ecs/Systems/TransformSystem.hpp"

namespace rpg::ecs {

struct ECS
{
    com::Bitset<MAX_ENTITY_COUNT>              entities;
    com::Array<ID, MAX_ENTITY_COUNT>           entitiesUsed;

    ComponentArrays<MAX_ENTITY_COUNT>          arrays;
    ComponentArrays<res::PrefabEnum::ENUM_END> prefabsArrays;
    
    void Update(float const dt, gpu::RenderData& renderData)
    {
        //? SYSTEMS
        TransformSystem(arrays, dt);
        RenderSystem(arrays, dt, renderData);
    }

    auto AddEntity() -> ID
    {
        const ID freeId = entities.FindFirstFreeBit();
        entities.Set(freeId, true);
        entitiesUsed.Append(freeId);
        return freeId;
    }

    auto AddEntity(res::PrefabEnum const& prefabEnum) -> ID
    {
        const ID freeId = AddEntity();
        arrays.SetComponentFrom(freeId, (ID) prefabEnum, prefabsArrays);

        //instantiate children (prefabs)
        if (auto* mainComponent = arrays.mainComponents.GetOptional(freeId)) {
            FOR_ARRAY(mainComponent->children, i){
                auto const prefabID = mainComponent->children[i];
                auto const childID  = AddEntity((res::PrefabEnum) prefabID);
                mainComponent->children[i] = childID;
            }
        }

        return freeId;
    }

    void RemoveEntity(ID const entityID)
    {
        dbg::Assert(false, "no impl yet");
        //entitiesUsed 
    }

};    

}//ns