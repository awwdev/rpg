#pragma once
#include "com/box/StringMap.hpp"
#include "ecs/Prefabs/PrefabEnum.hpp"
	
namespace rpg::ecs2 {

enum class ComponentDataEnum
{
    Children,
    Mesh,
    Material,
    ENUM_END
};

const com::EnumMap<ComponentDataEnum::ENUM_END, com::String<ecs2::PREFAB_FILE_LINE_LEN_MAX>> COMPONENTDATA_ENUM_TO_STR
{
    { ComponentDataEnum::Children,      "Children" },
    { ComponentDataEnum::Mesh,          "Mesh" },
    { ComponentDataEnum::Material,      "Material" },
};
const auto COMPONENTDATA_STR_TO_ENUM = com::StringMapFromEnumMap<ComponentDataEnum, ecs2::PREFAB_FILE_LINE_LEN_MAX>(COMPONENTDATA_ENUM_TO_STR);




struct ComponentDataPair
{

};

inline auto ConvertToComponentDataPair(chars_t line) -> const ComponentDataPair
{
        
    return {};
}

} //ns