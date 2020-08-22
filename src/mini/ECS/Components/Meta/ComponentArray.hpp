//https://github.com/awwdev

#pragma once
#include "mini/Box/Array.hpp"
#include "mini/Box/Bitset.hpp"

#include "mini/ECS/EntityID.hpp"
#include "mini/ECS/Components/Meta/ComponentTypes.hpp"

namespace mini::ecs
{
template<class COMPONENT, auto MAX_COUNT = MAX_ENTITY_COUNT>
struct ComponentArray
{
    box::Array<COMPONENT, MAX_COUNT> dense;
    ID componentLookup [MAX_COUNT];
    ID entityLookup    [MAX_COUNT];

    auto*       GetOptional(const ID entityID)       { return componentLookup[entityID] == NONE ? nullptr : &dense[componentLookup[entityID]]; }
    const auto* GetOptional(const ID entityID) const { return componentLookup[entityID] == NONE ? nullptr : &dense[componentLookup[entityID]]; }
    auto&       Get        (const ID entityID)       { return dense[componentLookup[entityID]]; }
    const auto& Get        (const ID entityID) const { return dense[componentLookup[entityID]]; }

    void SetAllToNone()
    {
        FOR_CARRAY(componentLookup, i) componentLookup[i] = NONE;
        FOR_CARRAY(entityLookup, i)    entityLookup[i]    = NONE;
    }

    template<class... CtorArgs>
    void AddComponent(const ID entityID, CtorArgs&&... args)
    {
        if (componentLookup[entityID] != NONE) {
            dbg::LogWarning("add component that alread exists");
            dense[componentLookup[entityID]] = { std::forward<CtorArgs>(args)... };
            return;
        }
        
        dense.Append(std::forward<CtorArgs>(args)...);
        componentLookup[entityID] = dense.count - 1;
        entityLookup[dense.count - 1] = entityID;
    }

    void CopyComponent(const ID entityID, const COMPONENT& other)
    {
        if (componentLookup[entityID] != NONE) {
            dense[componentLookup[entityID]] = other; 
            return;
        }

        dense.Append(other);
        componentLookup[entityID] = dense.count - 1;
        entityLookup[dense.count - 1] = entityID;
    }

};

}//NS