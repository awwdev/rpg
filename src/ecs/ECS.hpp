//https://github.com/awwdev

#pragma once
#include "com/box/Bitset.hpp"
#include "ecs/EntityID.hpp"
#include "ecs/ComponentsMeta/ComponentArrays.hpp"
#include "res/Prefab/PrefabEnum.hpp"

namespace rpg::ecs {

struct ECS
{
    com::Bitset<MAX_ENTITY_COUNT>     entities;
    ComponentArrays<MAX_ENTITY_COUNT> arrays;
    
    ID AddEntity()
    {
        const ID freeId = entities.FindFirstFreeBit();
        entities.Set(freeId, true);
        return freeId;
    }

    //ID AddEntity(const PrefabType& prefabType)
    //{
    //    const ID freeId = AddEntity();
    //    arrays.CopyComponents(freeId, (ID)prefabType, prefabsArrays);
    //    return freeId;
    //}

};    

}//ns