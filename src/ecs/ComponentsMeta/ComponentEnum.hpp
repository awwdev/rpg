//https://github.com/awwdev

#pragma once
#include "ecs/EntityID.hpp"
#include "com/box/StringMap.hpp"
#include "ecs/Prefabs/PrefabEnum.hpp"

namespace rpg::ecs {

enum class ComponentEnum : ecs::ID
{
    MainComponent, 
    ENUM_END
};

const com::EnumMap<ComponentEnum::ENUM_END, com::String<ecs2::PREFAB_FILE_LINE_LEN_MAX>> COMPONENT_ENUM_TO_STR
{
    { ComponentEnum::MainComponent, "MainComponent" },
};
const auto COMPONENT_STR_TO_ENUM = com::StringMapFromEnumMap<ComponentEnum, ecs2::PREFAB_FILE_LINE_LEN_MAX>(COMPONENT_ENUM_TO_STR);

}//NS