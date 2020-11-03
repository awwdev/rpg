//https://github.com/awwdev

#pragma once
#include "ecs/ComponentsMeta/ComponentArray.hpp"


namespace rpg::ecs {

template<auto MAX_COMPONENT_COUNT = MAX_ENTITY_COUNT>
struct ComponentArrays
{
    com::Bitset<ComponentEnum::ENUM_END> signatures[(idx_t) MAX_COMPONENT_COUNT];

    //? COMPONENT ARRAYS
    ComponentArray<TransformComponent, MAX_COMPONENT_COUNT>  transform_components;
    ComponentArray<RenderComponent   , MAX_COMPONENT_COUNT>  render_components;

    template<class... CtorArgs>
    void SetComponent(const ID entityID, const ComponentEnum componentType, CtorArgs&&... args)
    {
        signatures[entityID].Set(componentType, true);

        //? COMPONENT ADDING
        switch(componentType)
        {
            case ComponentEnum::TransformComponent: transform_components.SetComponent(entityID, std::forward<CtorArgs>(args)...); break;
            case ComponentEnum::RenderComponent: render_components.SetComponent(entityID, std::forward<CtorArgs>(args)...); break;
            default: dbg::Assert(false, "component missing");
        }
    }

    template<auto SRC_MAX_COUNT> //used both by serialization and prefab loading
    void CopyComponents(
    const ID desEntityID, 
    const ID srcEntityID, const ComponentArrays<SRC_MAX_COUNT>& srcComponentArrays)
    {
        const auto SetComponentOptional = [&](auto& dst, const auto& src){
            if (const auto* ptr = src.GetPtr(srcEntityID))
                dst.SetComponent(desEntityID, *ptr);
        };

        //? COMPONENT ADDING
        SetComponentOptional(transform_components, srcComponentArrays.transform_components);
        SetComponentOptional(render_components, srcComponentArrays.render_components);
    }

    void ReadBinaryFile()
    {
        transform_components.ReadBinaryFile();
        render_components.ReadBinaryFile();
    }

    void WriteBinaryFile() const
    {
        transform_components.WriteBinaryFile();
        render_components.WriteBinaryFile();
    }

    void Clear()
    {
        FOR_C_ARRAY(signatures, i)
            signatures[i].Clear();

        //? COMPONENT CLEAR
        transform_components.Clear();
        render_components.Clear();
    }

};

}//NS