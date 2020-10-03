//https://github.com/awwdev

#pragma once
#include "com/box/Bitset.hpp"
#include "ecs/EntityID.hpp"
#include "ecs/ComponentsMeta/ComponentArrays.hpp"
#include "res/Prefab/PrefabEnum.hpp"

namespace rpg::ecs {

struct ECS
{
    com::Bitset<MAX_ENTITY_COUNT>              entities;
    ComponentArrays<MAX_ENTITY_COUNT>          arrays;
    ComponentArrays<res::PrefabEnum::ENUM_END> prefabsArrays;
    
    auto AddEntity() -> ID
    {
        const ID freeId = entities.FindFirstFreeBit();
        entities.Set(freeId, true);
        return freeId;
    }

    auto AddEntity(res::PrefabEnum const& prefabEnum) -> ID
    {
        const ID freeId = AddEntity();
        arrays.SetComponentFrom(freeId, (ID) prefabEnum, prefabsArrays);

        //instantiate children (prefabs)
        if (auto const* mainComponent = arrays.mainComponents.GetOptional(freeId)) {
            FOR_ARRAY(mainComponent->children, i){
                auto const prefabID = mainComponent->children[i];
                AddEntity((res::PrefabEnum) prefabID);
            }
        }

        return freeId;
    }

};    

}//ns