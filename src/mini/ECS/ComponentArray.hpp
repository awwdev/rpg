//https://github.com/awwdev

#pragma once
#include "mini/Box/Array.hpp"
#include "mini/Box/Bitset.hpp"

#include "mini/ECS/EntityID.hpp"
#include "mini/ECS/ComponentTypes.hpp"
#include "mini/ECS/Components/C_RenderData.hpp"
#include "mini/ECS/Components/C_Transform.hpp"

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


    template<auto MAX_COUNT = MAX_ENTITY_COUNT>
    struct ComponentArrays
    {
        box::Bitset<ComponentType::ENUM_END> signatures[MAX_COUNT];

        //? COMPONENT ARRAYS
        ComponentArray<C_Transform, MAX_COUNT>  transforms;
        ComponentArray<C_RenderData, MAX_COUNT> renderData;

        ComponentArrays()
        {
            //? COMPONENT CLEARING
            transforms.SetAllToNone();
            renderData.SetAllToNone();
        }

        template<ComponentType TYPE, class... CtorArgs>
        void AddComponent(const ID entityID, CtorArgs&&... args)
        {
            //keep using enum and not struct type, for the bitset:
            signatures[entityID].Set(TYPE, true);

            //? COMPONENT ADDING
            if constexpr(TYPE == ComponentType::Transform)  transforms.AddComponent(entityID, std::forward<CtorArgs>(args)...);
            if constexpr(TYPE == ComponentType::RenderData) renderData.AddComponent(entityID, std::forward<CtorArgs>(args)...);
        }

        template<class... CtorArgs>
        void AddComponent(const ID entityID, const ComponentType componentType, CtorArgs&&... args)
        {
            //keep using enum and not struct type, for the bitset:
            signatures[entityID].Set(componentType, true);

            //? COMPONENT ADDING
            switch(componentType)
            {
                case ComponentType::Transform:  transforms.AddComponent(entityID, std::forward<CtorArgs>(args)...); break;
                case ComponentType::RenderData: renderData.AddComponent(entityID, std::forward<CtorArgs>(args)...); break;
            }
        }

        template<auto OTHER_MAX_COUNT>
        void CopyComponents(
            const ID entityID, 
            const ID otherEntityID, 
            const ComponentArrays<OTHER_MAX_COUNT>& other)
        {
            const auto CopyOptional = [&](auto& dst, const auto& src){
                if (const auto ptr = src.GetOptional(otherEntityID))
                    dst.CopyComponent(entityID, *ptr);
            };

            //? COMPONENT COPYING
            CopyOptional(transforms, other.transforms);
            CopyOptional(renderData, other.renderData);
        }

    };

}//NS