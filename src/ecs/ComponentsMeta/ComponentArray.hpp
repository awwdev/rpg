//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/box/Bitset.hpp"
#include "com/box/Optional.hpp"
#include "com/Types.hpp"
#include "com/utils/Utils.hpp"

#include "ecs/EntityID.hpp"
#include "ecs/ComponentsMeta/ComponentEnum.hpp"
#include "ecs/Components/TransformComponent.hpp"
#include "ecs/Components/RenderComponent.hpp"

namespace rpg::ecs {
    
template<typename COMPONENT_T, auto MAX_COMPONENT_COUNT = MAX_ENTITY_COUNT>
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

    //? constructor

    ComponentArray() 
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
        //!TODO
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

    constexpr auto GetPaths() const 
    {
        struct Paths 
        { 
            const char* const dense_path;
            const char* const component_lookup_path;
            const char* const entity_lookup_path;
        };

        if constexpr (std::is_same_v<COMPONENT_T, TransformComponent>) 
        {
            return Paths{
                .dense_path             = "tmp/transform_component.dense",
                .component_lookup_path  = "tmp/transform_component.component_lookup",
                .entity_lookup_path     = "tmp/transform_component.entity_lookup",
            };
        }

        if constexpr (std::is_same_v<COMPONENT_T, RenderComponent>) 
        {
            return Paths{
                .dense_path             = "tmp/render_component.dense",
                .component_lookup_path  = "tmp/render_component.component_lookup",
                .entity_lookup_path     = "tmp/render_component.entity_lookup",
            };
        }
    }

    void WriteBinaryFile() const
    {
        auto const [dense_path, component_lookup_path, entity_lookup_path] = GetPaths();
        dense.WriteBinaryFile(dense_path);
         com::WriteBinaryFile_C_Array(component_lookup_path, componentLookup, ArrayCount(componentLookup));
         com::WriteBinaryFile_C_Array(entity_lookup_path, entityLookup, ArrayCount(entityLookup));
    }

    void ReadBinaryFile()
    {
        auto const [dense_path, component_lookup_path, entity_lookup_path] = GetPaths();
        dense.ReadBinaryFile(dense_path);
         com::ReadBinaryFile_C_Array(component_lookup_path, componentLookup);
         com::ReadBinaryFile_C_Array(entity_lookup_path, entityLookup);
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