//https://github.com/awwdev

#pragma once
#include "ecs/ComponentsMeta/ComponentArray.hpp"
#include "ecs/Components/_Old/RenderComponent.hpp"
#include "ecs/Components/_Old/TransformComponent.hpp"

namespace rpg::ecs {

template<auto MAX_COUNT = MAX_ENTITY_COUNT> //! could be more on a 1->n relationship
struct ComponentArrays
{
    com::Bitset<ComponentEnum::ENUM_END> signatures[(idx_t) MAX_COUNT];

    //? COMPONENT ARRAYS
    ComponentArray<TransformComponent, MAX_COUNT>  transforms;
    ComponentArray<RenderComponent, MAX_COUNT> renderComponents;

    ComponentArrays()
    {
        //? COMPONENT CLEARING
        transforms.SetAllToNone();
        renderComponents.SetAllToNone();
    }

    template<ComponentEnum TYPE, class... CtorArgs>
    auto& AddComponent(const ID entityID, CtorArgs&&... args)
    {
        //this method is somewhat needed, I get compiler error if I use the runtime version of this method
        //I actually don't really comprehend why 
        signatures[entityID].Set(TYPE, true);

        //? COMPONENT ADDING
        //if constexpr(TYPE == ComponentEnum::TransformComponent)  return transforms.AddComponent(entityID, std::forward<CtorArgs>(args)...);
        //if constexpr(TYPE == ComponentEnum::RenderComponent) return renderComponents.AddComponent(entityID, std::forward<CtorArgs>(args)...);
    }

    template<class... CtorArgs>
    void AddComponent(const ID entityID, const ComponentEnum componentType, CtorArgs&&... args)
    {
        signatures[entityID].Set(componentType, true);

        //? COMPONENT ADDING
        switch(componentType)
        {
            //case ComponentEnum::RenderComponent: renderComponents.AddComponent(entityID, std::forward<CtorArgs>(args)...); break;
            //case ComponentEnum::TransformComponent:  transforms.AddComponent(entityID, std::forward<CtorArgs>(args)...); break;
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
        CopyOptional(renderComponents, other.renderComponents);
    }

};

}//NS