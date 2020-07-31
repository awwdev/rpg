//https://github.com/awwdev

#pragma once
#include "mini/Box/Bitset.hpp"
#include "mini/ECS/EntityID.hpp"
#include "mini/ECS/ComponentArray.hpp"
#include "mini/ECS/Prefabs.hpp"


namespace mini::ecs
{
    struct ECS
    {
        box::Bitset<MAX_ENTITY_COUNT>    entities;
        ComponentArrays<MAX_ENTITY_COUNT> arrays;
        Prefabs                           prefabs;
        
        ID AddEntity()
        {
            //const ID freeId = (removedEntities.Empty()) 
            //    ? entities.FindFirstFreeBit() 
            //    : removedEntities.RemoveLast();
            const ID freeId = entities.FindFirstFreeBit();
            entities.Set(freeId, true);
            //++entityCount;
            return freeId;
        }

        ID AddEntity(const PrefabType& prefabType)
        {
            ID id = AddEntity();
            arrays.CopyComponents(id, (ID)prefabType, prefabs.arrays);
            return id;
        }

        //ID AddEntity(const PrefabType& prefab)
        //{
        //    ID id = AddEntity();
        //    //copy components
        //    return id;
        //}
    };    

}//ns