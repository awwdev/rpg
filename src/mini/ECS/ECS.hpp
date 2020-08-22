//https://github.com/awwdev

#pragma once
#include "mini/Box/Bitset.hpp"
#include "mini/ECS/EntityID.hpp"
#include "mini/ECS/ComponentArrays.hpp"
#include "mini/ECS/Prefabs/PrefabTypes.hpp"

namespace mini::ecs {

struct ECS
{
    box::Bitset<MAX_ENTITY_COUNT>     entities;
    ComponentArrays<MAX_ENTITY_COUNT> arrays;
    ComponentArrays<PREFAB_COUNT_MAX> prefabsArrays;
    
    ID AddEntity()
    {
        const ID freeId = entities.FindFirstFreeBit();
        entities.Set(freeId, true);
        return freeId;
    }

    ID AddEntity(const PrefabType& prefabType)
    {
        const ID freeId = AddEntity();
        arrays.CopyComponents(freeId, (ID)prefabType, prefabsArrays);
        return freeId;
    }

};    

}//NS