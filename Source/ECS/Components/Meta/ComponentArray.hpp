//https://github.com/awwdev

#pragma once
#include "Common/Container/Array.hpp"
#include "Common/Container/Bitset.hpp"

#include "ECS/EntityID.hpp"
#include "ECS/Components/Meta/ComponentTypes.hpp"

namespace rpg::ecs
{
template<class COMPONENT, auto MAX_COUNT = MAX_ENTITY_COUNT>
struct ComponentArray
{
    com::Array<COMPONENT, MAX_COUNT> dense;
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
    COMPONENT& AddComponent(const ID entityID, CtorArgs&&... args)
    {
        if (componentLookup[entityID] != NONE) {
            dbg::LogWarning("add component that alread exists");
            dense[componentLookup[entityID]] = { std::forward<CtorArgs>(args)... };
            return dense[componentLookup[entityID]];
        }
        
        dense.Append(std::forward<CtorArgs>(args)...);
        componentLookup[entityID] = dense.count - 1;
        entityLookup[dense.count - 1] = entityID;
        return dense.Last();
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