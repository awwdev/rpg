//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "com/box/StringMap.hpp"
#include "res/Prefab/PrefabMeta.hpp"

namespace rpg::res {

enum class MeshEnum : u8
{
    None, 
    Sword_Grib,
    Sword_Blade,
    ENUM_END
};

const com::EnumMap<MeshEnum::ENUM_END, com::String<PREFAB_FILE_LINE_LEN_MAX>> MESH_ENUM_TO_STR
{
    { MeshEnum::Sword_Grib,   "Mesh_Sword_Grib" },
    { MeshEnum::Sword_Blade,  "Mesh_Sword_Blade" },
};
const auto MESH_STR_TO_ENUM = com::StringMapFromEnumMap<MeshEnum, PREFAB_FILE_LINE_LEN_MAX>(MESH_ENUM_TO_STR);


}//NS