//https://github.com/awwdev

#pragma once
#include "Common/Container/Bitset.hpp"
#include "ECS/EntityID.hpp"
#include "ECS/Components/Meta/ComponentArrays.hpp"
#include "ECS/Prefabs/PrefabTypes.hpp"

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