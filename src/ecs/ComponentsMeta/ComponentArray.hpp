//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/box/Bitset.hpp"
#include "com/box/Optional.hpp"
#include "com/Types.hpp"
#include "com/Utils.hpp"

#include "ecs/EntityID.hpp"
#include "ecs/ComponentsMeta/ComponentEnum.hpp"

namespace rpg::ecs {
    
template<class COMPONENT_T, auto MAX_COMPONENT_COUNT = MAX_ENTITY_COUNT>
struct ComponentArray
{
    //? data

    using COMPONENT = COMPONENT_T;

    com::Array<COMPONENT_T, MAX_COMPONENT_COUNT> dense;
    ID componentLookup [static_cast<idx_t>(MAX_COMPONENT_COUNT)];
    ID entityLookup    [static_cast<idx_t>(MAX_COMPONENT_COUNT)];

    auto*       GetPtr (const ID entityID)       { return componentLookup[entityID] == ECS_NULL ? nullptr : &dense[componentLookup[entityID]]; }
    const auto* GetPtr (const ID entityID) const { return componentLookup[entityID] == ECS_NULL ? nullptr : &dense[componentLookup[entityID]]; }
    auto&       Get    (const ID entityID)       { return dense[componentLookup[entityID]]; }
    const auto& Get    (const ID entityID) const { return dense[componentLookup[entityID]]; }

    ComponentEnum componentEnum;

    //? constructor

    ComponentArray(ComponentEnum const pComponentEnum) 
        : componentEnum { pComponentEnum }
    {
        SetLookupsToNull();    
        //TODO: just using 0 as null entity is probably better than max value ? 
    }

    //? methods

    template<class... CtorArgs>
    COMPONENT_T& SetComponent(const ID entityID, CtorArgs&&... args)
    {
        //overwrite
        if (componentLookup[entityID] != ECS_NULL) {
            dbg::LogWarning("add component that alread exists");
            dense[componentLookup[entityID]] = { std::forward<CtorArgs>(args)... };
            return dense[componentLookup[entityID]];
        }
        //new
        dense.AppendElement(std::forward<CtorArgs>(args)...);
        componentLookup[entityID] = dense.Count() - 1;
        entityLookup[dense.Count() - 1] = entityID;
        return dense.Last();
    }

    void Clear()
    {
        dense.Clear();
        SetLookupsToNull();
    }

    void Remove(ID const entityID)
    {
        //get the component in dense array
        auto const denseIdx = componentLookup[entityID];

        //nullify lookups
        componentLookup[entityID] = ECS_NULL;
        entityLookup[denseIdx] = ECS_NULL;

        //swap element at idx with the last element, and remove last element
        //store last element lookups first
        auto const swapped_entityID = entityLookup[dense.Count() - 1];
        if (denseIdx < dense.Count() - 1) 
        {
            componentLookup[swapped_entityID] = denseIdx;
            entityLookup[denseIdx] = swapped_entityID;
        }

        dense.Remove_Swap(denseIdx);      
    }   

    //? serialization

    void WriteBinaryFile() const
    {
        dbg::Assert(COMPONENT_SERIALIZATION_PATHS.Contains(componentEnum), "component path missing");
        auto const& paths = COMPONENT_SERIALIZATION_PATHS.Get(componentEnum);

        dense.WriteBinaryFile(paths.dense.Data());
        com::WriteBinaryFile_C_Array(paths.componentLookup.Data(), componentLookup, ArrayCount(componentLookup));
        com::WriteBinaryFile_C_Array(paths.entityLookup.Data(), entityLookup, ArrayCount(entityLookup));
    }

    void ReadBinaryFile()
    {
        dbg::Assert(COMPONENT_SERIALIZATION_PATHS.Contains(componentEnum), "component path missing");
        auto const& paths = COMPONENT_SERIALIZATION_PATHS.Get(componentEnum);

        dense.ReadBinaryFile(paths.dense.Data());
        com::ReadBinaryFile_C_Array(paths.componentLookup.Data(), componentLookup);
        com::ReadBinaryFile_C_Array(paths.entityLookup.Data(), entityLookup);
    }

private:

    //? internal helper

    void SetLookupsToNull()
    {
        for(idx_t i = 0; i < (idx_t) MAX_COMPONENT_COUNT; ++i) {
            componentLookup[i] = ECS_NULL;
            entityLookup[i]    = ECS_NULL;
        } 
    }

};

}//NS