//https://github.com/awwdev

#pragma once
#include "mini/Box/StringMap.hpp"

namespace mini::ecs {

enum class PrefabType
{
    Sword,
    Stone,
    Grass,
    Tree,
    ENUM_END
};

constexpr ID PREFAB_COUNT_MAX = (ID)PrefabType::ENUM_END;
constexpr auto PREFAB_MAX_STR_LEN = 100;

const box::EnumMap<PrefabType::ENUM_END, box::String<PREFAB_MAX_STR_LEN>> PREFAB_ENUM_TO_STR
{
    { PrefabType::Sword, "Sword" },
    { PrefabType::Stone, "Stone" },
    { PrefabType::Grass, "Grass" },
    { PrefabType::Tree , "Tree" },
};
const auto PREFAB_STR_TO_ENUM = box::StringMapFromEnumMap<PrefabType, PREFAB_MAX_STR_LEN>(PREFAB_ENUM_TO_STR);

}//NS