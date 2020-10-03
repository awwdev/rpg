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
    void SetComponent(const ID entityID, const ComponentEnum componentType, CtorArgs&&... args)
    {
        signatures[entityID].Set(componentType, true);

        //? COMPONENT ADDING
        switch(componentType)
        {
            case ComponentEnum::MainComponent: mainComponents.SetComponent(entityID, std::forward<CtorArgs>(args)...); break;
            default: dbg::Assert(false, "component to add not impl");
        }
    }

    template<auto SRC_MAX_COUNT>
    void SetComponentFrom(
    const ID desEntityID, 
    const ID srcEntityID, const ComponentArrays<SRC_MAX_COUNT>& srcComponentArrays)
    {
        const auto SetComponentOptional = [&](auto& dst, const auto& src){
            if (const auto ptr = src.GetOptional(srcEntityID))
                dst.SetComponent(desEntityID, *ptr);
        };

        //? COMPONENT ADDING
        SetComponentOptional(mainComponents, srcComponentArrays.mainComponents);
    }

    void Clear()
    {
        FOR_C_ARRAY(signatures, i)
            signatures[i].Clear();

        //? COMPONENT CLEAR
        mainComponents.Clear();
    }

};

}//NS