//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "com/box/StringMap.hpp"

namespace rpg::res {

enum class MeshEnum : u8
{
    Sword_Grib,
    Sword_Blade,
    ENUM_END
};

//constexpr ID PREFAB_COUNT_MAX = (ID)PrefabType::ENUM_END;
//constexpr auto PREFAB_MAX_STR_LEN = 100;
//
//const com::EnumMap<PrefabType::ENUM_END, com::String<PREFAB_MAX_STR_LEN>> PREFAB_ENUM_TO_STR
//{
//    { PrefabType::Sword, "Sword" },
//    { PrefabType::Stone, "Stone" },
//    { PrefabType::Grass, "Grass" },
//    { PrefabType::Tree , "Tree" },
//};
//const auto PREFAB_STR_TO_ENUM = com::StringMapFromEnumMap<PrefabType, PREFAB_MAX_STR_LEN>(PREFAB_ENUM_TO_STR);

}//NS