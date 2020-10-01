//https://github.com/awwdev

#pragma once
#include "com/box/Bitset.hpp"
#include "ecs/EntityID.hpp"
#include "ecs/ComponentsMeta/ComponentArrays.hpp"
#include "ecs/Prefabs/_Old/PrefabTypes.hpp"

namespace rpg::ecs {

struct ECS
{
    com::Bitset<MAX_ENTITY_COUNT>     entities;
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