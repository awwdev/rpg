//https://github.com/awwdev

#pragma once
#include "com/box/Bitset.hpp"
#include "com/box/String.hpp"
#include "com/mem/Allocator.hpp"
#include "res/Prefab/PrefabEnum.hpp"

#include "ecs/ComponentsMeta/ComponentArrays.hpp"
#include "gpu/RenderData/RenderData.hpp"

#include "ecs/Systems/RenderSystem.hpp"
#include "ecs/Systems/TransformSystem.hpp"

namespace rpg::ecs {

struct ECS
{
    //? data arrays

    com::Bitset<MAX_ENTITY_COUNT> entities;
    com::Array<ID, MAX_ENTITY_COUNT> entitiesParentless;
    ComponentArrays<MAX_ENTITY_COUNT> arrays;
    ComponentArrays<res::PrefabEnum::ENUM_END> prefabsArrays;


    //? update

    void Update(float const dt, gpu::RenderData& renderData)
    {
        //? system updates
        TransformSystem(arrays, dt);
        RenderSystem(arrays, dt, renderData);
    }


    //? adding

    ID AddEntity()
    {
        auto const entityID = RegisterEntity();
        entitiesParentless.AppendElement(entityID);
        return entityID;
    }

    ID AddEntity(res::PrefabEnum const& prefabEnum)
    {
        auto const entityID = AddEntity();
        InitPrefab(entityID, prefabEnum);
        return entityID;
    }

    ID AddChild(ID const parentID)
    {
        auto const childID = RegisterEntity();
        RegisterChild(childID, parentID);
        return childID;
    }

    ID AddChild(ID const parentID, res::PrefabEnum const& prefabEnum)
    {
        auto const childID = AddChild(parentID);
        InitPrefab(childID, prefabEnum);
        return childID;
    }

    //? removing

    void RemoveEntity(ID const entityID)
    {
        entities.Set(entityID, false);

        if (auto const found = entitiesParentless.Find(entityID))
            entitiesParentless.Remove_PreserveOrder(*found);

        //TODO: check signature to get active components
        arrays.mainComponents.Remove(entityID);
        arrays.nameComponents.Remove(entityID);
    }

private:

    //? internal helpers

    auto RegisterEntity() -> ID
    {
        auto const optionalEntityID = entities.FindFreeBit_Optional();
        dbg::Assert(optionalEntityID, "no free entity id");
        entities.Set(*optionalEntityID, true);
        return *optionalEntityID;
    }

    void RegisterChild(ID const childID, ID const parentID)
    {
        dbg::Assert(arrays.mainComponents.GetPtr(parentID), "trying to add child to non parent (no main component)");
        auto& parentMainComponent = arrays.mainComponents.Get(parentID);
        parentMainComponent.children.AppendElement(childID);
    }

    void InitPrefab(ID const entityID, res::PrefabEnum const prefabEnum)
    {
        arrays.CopyComponents(entityID, (ID) prefabEnum, prefabsArrays);
        //instantiate potential child prefabs (note that the entity has child prefab IDs (copied) and not real IDs yet)
        if (auto* mainComponent = arrays.mainComponents.GetPtr(entityID)) {
            auto const prefabChildren = mainComponent->children;
            mainComponent->children.Clear();
            //clear prefab children and add real children 
            FOR_ARRAY(prefabChildren, i) { 
                auto const prefabChildID = (res::PrefabEnum) prefabChildren[i];
                AddChild(entityID, prefabChildID);
            }
        }
    }

};    

}//ns