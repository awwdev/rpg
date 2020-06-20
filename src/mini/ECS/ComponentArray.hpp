//https://github.com/awwdev

#pragma once
#include "mini/Box/POD_Array.hpp"
#include "mini/Box/IndexMap.hpp"
#include "mini/Box/String.hpp"
#include "mini/Math/Matrix.hpp"

#include "mini/ECS/Components.hpp"
#include "mini/ECS/EntityID.hpp"
#include "mini/Utils/CharsView.hpp"

namespace mini::ecs
{
    template<u32 MAX_COUNT, class COMPONENT>
    struct ComponentArray
    {
        box::Array<COMPONENT, MAX_COUNT> dense;
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

        template<class... CtorArgs>
        void AddComponent(const ID entityID, CtorArgs&&... args)
        {
            if (componentLookup[entityID] != NONE) {
                WARN("adding component that already exists");
                return;
            }
            
            dense.Append(std::forward<CtorArgs>(args)...);
            componentLookup[entityID] = dense.count - 1;
            entityLookup[dense.count - 1] = entityID;
        }

        void RemoveComponent(const ID entityID)
        {
            if (componentLookup[entityID] == NONE) {
                WARN("removing component that does not exist");
                return;
            }
            //TODO:
        }
    };

    template<u32 MAX_COUNT>
    struct ComponentArrays
    {
        box::Bitset<ComponentType::ENUM_END> signatures[MAX_COUNT];

        ComponentArray<MAX_COUNT, C_Transform>  transforms;
        ComponentArray<MAX_COUNT, C_RenderData> renderData;
        ComponentArray<MAX_COUNT, C_UI>         uiData;

        template<class... CtorArgs>
        void AddComponent(const ID entityID, const ComponentType& type, CtorArgs&&... args)
        {
            signatures[entityID].Set<true>(type);
            switch(type)
            {
                case ComponentType::Transform:  transforms.AddComponent(entityID, std::forward<CtorArgs>(args)...);     break; 
                case ComponentType::RenderData: renderData.AddComponent(entityID, std::forward<CtorArgs>(args)...);     break;
                case ComponentType::UI:         uiData.AddComponent(entityID, std::forward<CtorArgs>(args)...);         break;
            }
        }
    };

}//ns

//TODO: make 0 the default for non existent lookup value (NONE)