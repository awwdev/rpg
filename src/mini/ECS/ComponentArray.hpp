//https://github.com/awwdev

#pragma once
#include "mini/Box/POD_Array.hpp"
#include "mini/Box/IndexMap.hpp"
#include "mini/Box/String.hpp"
#include "mini/Math/Matrix.hpp"

#include "mini/ECS/Components.hpp"
#include "mini/ECS/EntityID.hpp"

namespace mini::ecs
{
    enum ComponentType
    {
        Transform, 
        RenderData,
        ENUM_END
    };

    using ComponentTypeToStringMap = box::IndexMap<box::String<20>, ComponentType::ENUM_END>;
    using P = ComponentTypeToStringMap::Pair_t;
    const ComponentTypeToStringMap ComponentTypeToString 
    {
        P{ ComponentType::Transform,  "Transform"  },
        P{ ComponentType::RenderData, "RenderData" },
    };

    template<u32 MAX_COUNT, class COMPONENT>
    struct ComponentArray
    {
        box::POD_Array<COMPONENT, MAX_COUNT> dense;
        ID componentLookup [MAX_COUNT];
        ID entityLookup    [MAX_COUNT];

        auto*       GetOptional(const ID entityID)       { return componentLookup[entityID] == NONE ? nullptr : &dense[componentLookup[entityID]]; }
        const auto* GetOptional(const ID entityID) const { return componentLookup[entityID] == NONE ? nullptr : &dense[componentLookup[entityID]]; }
        auto&       Get        (const ID entityID)       { return dense[componentLookup[entityID]]; }
        const auto& Get        (const ID entityID) const { return dense[componentLookup[entityID]]; }

        ComponentArray()
        {
            FOR_CARRAY(componentLookup, i) componentLookup[i] = NONE;
            FOR_CARRAY(entityLookup, i)    entityLookup[i]    = NONE;
        }
    };

    template<u32 MAX_COUNT>
    struct ComponentArrays
    {
        ComponentArray<MAX_COUNT, C_Transform>  transforms;
        ComponentArray<MAX_COUNT, C_RenderData> renderData;
    };

}//ns

//TODO: make 0 the default for non existent lookup value (NONE)
//TODO: make map without byte array