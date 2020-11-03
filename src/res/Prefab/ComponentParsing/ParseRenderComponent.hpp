//https://github.com/awwdev

#pragma once
#include "ecs/Components/RenderComponent.hpp"
#include "res/Prefab/ComponentParsing/ComponentData.hpp"

namespace rpg::res {

inline auto ParseRenderComponent(ComponentDataPairs const& pairs)
{
    ecs::RenderComponent component {};
    FOR_ARRAY(pairs, i) 
    {
        auto const [key, val, componentDataEnum] = pairs[i].get_data();
        switch(componentDataEnum)
        {
            case ComponentDataEnum::Mesh:      
            {
                auto const meshEnum = res::MESH_STR_TO_ENUM.GetOptional(val);
                dbg::Assert(meshEnum, "no meshEnum");
                component.mesh_enum = *meshEnum;
            }
            break;

            default: dbg::Assert(false, "component data enum missing / wrong");
        }
    }
    return component;
}

}//ns