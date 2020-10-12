//https://github.com/awwdev

#pragma once
#include "com/box/Bitset.hpp"
#include "com/box/String.hpp"
#include "res/Prefab/PrefabEnum.hpp"

#include "ecs/ComponentsMeta/ComponentArrays.hpp"
#include "gpu/RenderData/RenderData.hpp"

#include "ecs/Systems/RenderSystem.hpp"
#include "ecs/Systems/TransformSystem.hpp"

#include <fstream>

namespace rpg::ecs {

struct ECS
{
    com::Bitset<MAX_ENTITY_COUNT>              entities;
    com::Array<ID, MAX_ENTITY_COUNT>           entitiesTopLevel;
    ComponentArrays<MAX_ENTITY_COUNT>          arrays;
    ComponentArrays<res::PrefabEnum::ENUM_END> prefabsArrays;


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
        entitiesTopLevel.AppendElement(entityID);
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
    //TODO: remove entity, removed entity array


    //? serialization

    void Save() const 
    {
        entitiesTopLevel.WriteBinaryFile("out/tmp/entitiesTopLevel.ecs");
        entities.WriteBinaryFile("out/tmp/entities.ecs");
        arrays.SaveComponents();

        auto blockPtr = entitiesTopLevel.WriteBinaryMemory();
        //TODO: could be fine that every system stores own history and you can revert what you like
    }

    void Load()
    {
        entitiesTopLevel.ReadBinaryFile("out/tmp/entitiesTopLevel.ecs");
        entities.ReadBinaryFile("out/tmp/entities.ecs");
        arrays.LoadComponents();
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