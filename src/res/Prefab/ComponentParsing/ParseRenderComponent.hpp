//https://github.com/awwdev

#pragma once
#include "ecs/Components/RenderComponent.hpp"
#include "res/Prefab/ComponentParsing/ComponentMember.hpp"

namespace rpg::res {

inline auto ParseRenderComponent(ComponentMemberPairs const& pairs)
{
    ecs::RenderComponent component {};
    FOR_ARRAY(pairs, i) 
    {
        auto const [key, val, componentDataEnum] = pairs[i].get_data();
        switch(componentDataEnum)
        {
            case ComponentMemberEnum::Mesh:      
            {
                auto const& meshEnum = res::MESH_STR_TO_ENUM.Get(val);
                component.meshEnum = meshEnum;
            }
            break;

            case ComponentMemberEnum::Material:      
            {
                auto const& matEnum = res::MESH_MATERIAL_STR_TO_ENUM.Get(val);
                component.materialEnum = matEnum;
            }
            break;

            default: dbg::Assert(false, "component data enum missing / wrong");
        }
    }
    return component;
}

}//ns