//https://github.com/awwdev

#pragma once
#include "com/box/StringMap.hpp"
#include "dbg/Assert.hpp"
#include "ecs/Components/Meta/ComponentParsing.hpp"
#include "ecs/Prefabs/PrefabEnum.hpp"

namespace rpg::ecs {



enum class ComponentEnum
{
    TransformComponent, 
    RenderComponent,
    ENUM_END
};

const com::EnumMap<ComponentEnum::ENUM_END, com::String<ecs2::PREFAB_FILE_LINE_LEN_MAX>> COMPONENT_ENUM_TO_STR
{
    { ComponentEnum::TransformComponent,    "TransformComponent" },
    { ComponentEnum::RenderComponent,       "RenderComponent" },
};
const auto COMPONENT_STR_TO_ENUM = com::StringMapFromEnumMap<ComponentEnum, ecs2::PREFAB_FILE_LINE_LEN_MAX>(COMPONENT_ENUM_TO_STR);



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


}//NS