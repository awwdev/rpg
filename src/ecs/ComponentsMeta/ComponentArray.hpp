//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/box/Bitset.hpp"

#include "ecs/EntityID.hpp"
#include "ecs/ComponentsMeta/ComponentEnum.hpp"

#include <fstream>

namespace rpg::ecs {
    
template<class COMPONENT_T, auto MAX_COMPONENT_COUNT = MAX_ENTITY_COUNT>
struct ComponentArray
{
    using COMPONENT = COMPONENT_T;

    com::Array<COMPONENT_T, MAX_COMPONENT_COUNT> dense;
    ID componentLookup [(idx_t) MAX_COMPONENT_COUNT];
    ID entityLookup    [(idx_t) MAX_COMPONENT_COUNT];

    auto*       GetPtr (const ID entityID)       { return componentLookup[entityID] == ECS_NULL ? nullptr : &dense[componentLookup[entityID]]; }
    const auto* GetPtr (const ID entityID) const { return componentLookup[entityID] == ECS_NULL ? nullptr : &dense[componentLookup[entityID]]; }
    auto&       Get    (const ID entityID)       { return dense[componentLookup[entityID]]; }
    const auto& Get    (const ID entityID) const { return dense[componentLookup[entityID]]; }

    com::String<30> filename;

    ComponentArray(chars_t name) 
        : filename { name }
    {
        Clear(); //used for initialization          
    }

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
        for(idx_t i = 0; i < (idx_t) MAX_COMPONENT_COUNT; ++i) {
            componentLookup[i] = ECS_NULL;
            entityLookup[i]    = ECS_NULL;
        }  
    }

    void Save()
    {
        //dense
        {
            com::String<100> path { "res/ECS/" }; 
            path.AppendString(filename);
            path.AppendArray("_dense.ecs");
            dense.WriteBinaryFile(path.Data());
        }

        //componentLookup
        {
            com::String<100> path { "res/ECS/" }; 
            path.AppendString(filename);
            path.AppendArray("_componentLookup.ecs");

            auto file = std::ofstream(path.Data(), std::ios::binary);
            dbg::Assert(file.is_open(), "cannot open file");

            file.write(reinterpret_cast<char const*>(componentLookup), MAX_COMPONENT_COUNT * sizeof(ID));
        }

        //entityLookup
        {
            com::String<100> path { "res/ECS/" }; 
            path.AppendString(filename);
            path.AppendArray("_entityLookup.ecs");

            auto file = std::ofstream(path.Data(), std::ios::binary);
            dbg::Assert(file.is_open(), "cannot open file");

            file.write(reinterpret_cast<char const*>(entityLookup), MAX_COMPONENT_COUNT * sizeof(ID));
        }
    }

    void Load()
    {
        //dense
        {
            com::String<100> path { "res/ECS/" }; 
            path.AppendString(filename);
            path.AppendArray("_dense.ecs");
            dense.ReadBinaryFile(path.Data());
        }

        //componentLookup
        {
            com::String<100> path { "res/ECS/" }; 
            path.AppendString(filename);
            path.AppendArray("_componentLookup.ecs");

            auto file = std::ifstream(path.Data(), std::ios::binary);
            dbg::Assert(file.is_open(), "cannot open file");

            file.read(reinterpret_cast<char*>(componentLookup), MAX_COMPONENT_COUNT * sizeof(ID));
        }

        //entityLookup
        {
            com::String<100> path { "res/ECS/" }; 
            path.AppendString(filename);
            path.AppendArray("_entityLookup.ecs");

            auto file = std::ifstream(path.Data(), std::ios::binary);
            dbg::Assert(file.is_open(), "cannot open file");

            file.read(reinterpret_cast<char*>(entityLookup), MAX_COMPONENT_COUNT * sizeof(ID));
        }
    }

};

}//NS