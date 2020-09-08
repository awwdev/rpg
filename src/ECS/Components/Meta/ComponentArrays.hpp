//https://github.com/awwdev

#pragma once
#include "ECS/Components/Meta/ComponentArray.hpp"
#include "ECS/Components/C_RenderData.hpp"
#include "ECS/Components/C_Transform.hpp"

namespace rpg::ecs {

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
    auto& AddComponent(const ID entityID, CtorArgs&&... args)
    {
        //this method is somewhat needed, I get compiler error if I use the runtime version of this method
        //I actually don't really comprehend why 
        signatures[entityID].Set(TYPE, true);

        //? COMPONENT ADDING
        if constexpr(TYPE == ComponentType::Transform)  return transforms.AddComponent(entityID, std::forward<CtorArgs>(args)...);
        if constexpr(TYPE == ComponentType::RenderData) return renderData.AddComponent(entityID, std::forward<CtorArgs>(args)...);
    }

    template<class... CtorArgs>
    void AddComponent(const ID entityID, const ComponentType componentType, CtorArgs&&... args)
    {
        signatures[entityID].Set(componentType, true);

        //? COMPONENT ADDING
        switch(componentType)
        {
            case ComponentType::RenderData: renderData.AddComponent(entityID, std::forward<CtorArgs>(args)...); break;
            case ComponentType::Transform:  transforms.AddComponent(entityID, std::forward<CtorArgs>(args)...); break;
            default: break;
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