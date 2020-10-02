//https://github.com/awwdev

#pragma once
#include "ecs/EntityID.hpp"
#include "com/box/StringMap.hpp"
#include "res/Prefab/PrefabEnum.hpp"

namespace rpg::ecs {

enum class ComponentEnum : ecs::ID
{
    MainComponent, 
    ENUM_END
};

const com::EnumMap<ComponentEnum::ENUM_END, com::String<res::PREFAB_FILE_LINE_LEN_MAX>> COMPONENT_ENUM_TO_STR
{
    { ComponentEnum::MainComponent, "MainComponent" },
};
const auto COMPONENT_STR_TO_ENUM = com::StringMapFromEnumMap<ComponentEnum, res::PREFAB_FILE_LINE_LEN_MAX>(COMPONENT_ENUM_TO_STR);

}//NS