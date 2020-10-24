//https://github.com/awwdev

#pragma once
#include "ecs/EntityID.hpp"
#include "res/Prefab/PrefabMeta.hpp"
#include "com/box/StringMap.hpp"

namespace rpg::res {

enum class PrefabEnum : ecs::ID
{
    Cube,
    Circle,
    Sword,
    Sword_Grib,
    Sword_Blade,
    Grass,
    ENUM_END
};

constexpr auto PREFAB_COUNT_MAX = (ecs::ID) PrefabEnum::ENUM_END;

const com::EnumMap<PrefabEnum::ENUM_END, com::String<PREFAB_FILE_LINE_LEN_MAX>> PREFAB_ENUM_TO_STR
{
    { PrefabEnum::Cube,         "Cube" },
    { PrefabEnum::Circle,       "Circle" },
    { PrefabEnum::Sword,        "Sword" },
    { PrefabEnum::Sword_Grib,   "Sword_Grib" },
    { PrefabEnum::Sword_Blade,  "Sword_Blade" },
    { PrefabEnum::Grass,        "Grass" },
};
const auto PREFAB_STR_TO_ENUM = com::StringMapFromEnumMap<PrefabEnum, PREFAB_FILE_LINE_LEN_MAX>(PREFAB_ENUM_TO_STR);

}//NS