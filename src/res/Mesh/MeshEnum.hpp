//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "com/box/StringMap.hpp"
#include "res/Prefab/PrefabMeta.hpp"
#include "res/Mesh/MeshMaterialEnum.hpp"

namespace rpg::res {

enum class MeshEnum : u8
{
    None, 
    Sword_Grib,
    Sword_Blade,
    Grass, 
    Sword2_Grib,
    Sword2_Blade,
    ENUM_END
};

const com::EnumMap<MeshEnum::ENUM_END, com::String<PREFAB_FILE_LINE_LEN_MAX>> MESH_ENUM_TO_STR
{
    { MeshEnum::Sword_Grib,     "Mesh_Sword_Grib" },
    { MeshEnum::Sword_Blade,    "Mesh_Sword_Blade" },
    { MeshEnum::Grass,          "Mesh_Grass" },
    { MeshEnum::Sword2_Grib,    "Mesh_Sword2_Grib" },
    { MeshEnum::Sword2_Blade,   "Mesh_Sword2_Blade" },
};
const auto MESH_STR_TO_ENUM = com::StringMapFromEnumMap<MeshEnum, PREFAB_FILE_LINE_LEN_MAX>(MESH_ENUM_TO_STR);


//TODO: both mesh paths and mesh materials should be loaded externally

inline com::EnumMap<MeshEnum::ENUM_END, com::String<PREFAB_FILE_LINE_LEN_MAX>> MESH_PATHS
{
    { MeshEnum::Sword_Grib,     "res/Meshes/Sword_Grib.mesh" },
    { MeshEnum::Sword_Blade,    "res/Meshes/Sword_Blade.mesh" },
    { MeshEnum::Grass,          "res/Meshes/Grass.mesh" },
    { MeshEnum::Sword2_Grib,    "res/Meshes/Sword2_Grib.mesh" },
    { MeshEnum::Sword2_Blade,   "res/Meshes/Sword2_Blade.mesh" },
};

inline com::EnumMap<MeshEnum::ENUM_END, MeshMaterialEnum> MESH_MATERIAL_GROUPS
{
    { MeshEnum::Sword_Grib,     MeshMaterialEnum::Default },
    { MeshEnum::Sword_Blade,    MeshMaterialEnum::Metal   },
    { MeshEnum::Sword2_Grib,    MeshMaterialEnum::Default },
    { MeshEnum::Sword2_Blade,   MeshMaterialEnum::Metal   },
    { MeshEnum::Grass,          MeshMaterialEnum::Foliage },
};

}//NS