//https://github.com/awwdev

#pragma once
#include "ecs/Components/TransformComponent.hpp"
#include "res/Prefab/ComponentParsing/ComponentData.hpp"

namespace rpg::res {

inline auto ParseTransformComponent(ComponentDataPairs const& pairs)
{
    ecs::TransformComponent transformComponent {};
    FOR_ARRAY(pairs, i) 
    {
        auto const [key, val, componentDataEnum] = pairs[i].get_data();
        switch(componentDataEnum)
        {
            case ComponentDataEnum::Children:
            {
                const auto values = ValStrToValArray<3, 100>(val);     
                dbg::Assert(!values.Empty(), "values are empty");
                FOR_ARRAY(values, i){
                    auto const prefabEnum = res::PREFAB_STR_TO_ENUM.GetOptional(values[i].Data());
                    dbg::Assert(prefabEnum, "child prefab enum wrong");
                    transformComponent.children.AppendElement((ID) *prefabEnum);
                }
            }
            break;

            case ComponentDataEnum::Scale:
            {
                const auto values = ValStrToValArray<3, 10>(val);   
                dbg::Assert(!values.Empty(), "values are empty");
                FOR_ARRAY(values, i){
                    transformComponent.scale[0][i] = std::atof(values[i].Data());
                }
            }
            break;

            default: dbg::Assert(false, "component data enum missing / wrong");
        }
    }
    return transformComponent;
}

}//ns