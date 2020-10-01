//https://github.com/awwdev

#pragma once
#include "ecs/ComponentsMeta/ComponentArray.hpp"
#include "ecs/Components/MainComponent.hpp"

namespace rpg::ecs {

template<auto MAX_COMPONENT_COUNT = MAX_ENTITY_COUNT>
struct ComponentArrays
{
    com::Bitset<ComponentEnum::ENUM_END> signatures[(idx_t) MAX_COMPONENT_COUNT];

    //? COMPONENT ARRAYS
    ComponentArray<MainComponent, MAX_COMPONENT_COUNT>  mainComponents;

    template<class... CtorArgs>
    void AddComponent(const ID entityID, const ComponentEnum componentType, CtorArgs&&... args)
    {
        signatures[entityID].Set(componentType, true);

        //? COMPONENT ADDING
        switch(componentType)
        {
            case ComponentEnum::MainComponent: mainComponents.AddComponent(entityID, std::forward<CtorArgs>(args)...); break;
            default: dbg::Assert(false, "component to add not impl");
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
        //TODO
        //CopyOptional(transforms, other.transforms);
        //CopyOptional(renderComponents, other.renderComponents);
    }

};

}//NS