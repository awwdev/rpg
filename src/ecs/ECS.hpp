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
    com::Array<ID, MAX_ENTITY_COUNT>           entitiesTopLevel;
    ComponentArrays<MAX_ENTITY_COUNT>          arrays;
    ComponentArrays<res::PrefabEnum::ENUM_END> prefabsArrays;


    void Update(float const dt, gpu::RenderData& renderData)
    {
        //? SYSTEMS
        TransformSystem(arrays, dt);
        RenderSystem(arrays, dt, renderData);
    }


    ID AddEntity()
    {
        auto const entityID = RegisterEntity();
        entitiesTopLevel.Append(entityID);
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

    //TODO: remove entity, removed entity array

private:
    auto RegisterEntity() -> ID
    {
        auto const optionalEntityID = entities.FindFreeBitOptional();
        dbg::Assert(optionalEntityID, "no free entity id");
        entities.Set(*optionalEntityID, true);
        return *optionalEntityID;
    }

    void RegisterChild(ID const childID, ID const parentID)
    {
        dbg::Assert(arrays.mainComponents.GetPtr(parentID), "trying to add child to non parent (no main component)");
        auto& parentMainComponent = arrays.mainComponents.Get(parentID);
        parentMainComponent.children.Append(childID);
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