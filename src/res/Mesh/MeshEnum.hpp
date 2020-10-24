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
    //primitives
    Cube,
    Circle,
    //meshes
    Sword1_Grib,
    Sword1_Blade,
    Sword2_Grib,
    Sword2_Blade,
    Grass, 
    ENUM_END
};

const com::EnumMap<MeshEnum::ENUM_END, com::String<PREFAB_FILE_LINE_LEN_MAX>> MESH_ENUM_TO_STR
{
    { MeshEnum::None,           "Mesh_None" },
    { MeshEnum::Cube,           "Mesh_Cube" },
    { MeshEnum::Circle,         "Mesh_Circle" },
    { MeshEnum::Sword1_Grib,    "Mesh_Sword1_Grib" },
    { MeshEnum::Sword1_Blade,   "Mesh_Sword1_Blade" },
    { MeshEnum::Sword2_Grib,    "Mesh_Sword2_Grib" },
    { MeshEnum::Sword2_Blade,   "Mesh_Sword2_Blade" },
    { MeshEnum::Grass,          "Mesh_Grass" },
};
const auto MESH_STR_TO_ENUM = com::StringMapFromEnumMap<MeshEnum, PREFAB_FILE_LINE_LEN_MAX>(MESH_ENUM_TO_STR);


//TODO: both mesh paths and mesh materials should be loaded externally

inline com::EnumMap<MeshEnum::ENUM_END, com::String<PREFAB_FILE_LINE_LEN_MAX>> MESH_PATHS
{
    { MeshEnum::Cube,           "res/Meshes/Cube.mesh"         },
    { MeshEnum::Circle,         "res/Meshes/Circle.mesh"         },
    { MeshEnum::Sword1_Grib,    "res/Meshes/Sword1_Grib.mesh"  },
    { MeshEnum::Sword1_Blade,   "res/Meshes/Sword1_Blade.mesh" },
    { MeshEnum::Sword2_Grib,    "res/Meshes/Sword2_Grib.mesh"  },
    { MeshEnum::Sword2_Blade,   "res/Meshes/Sword2_Blade.mesh" },
    { MeshEnum::Grass,          "res/Meshes/Grass.mesh"        },
};

inline com::EnumMap<MeshEnum::ENUM_END, MeshMaterialEnum> MESH_MATERIAL_GROUPS
{
    { MeshEnum::Cube,           MeshMaterialEnum::Default },
    { MeshEnum::Circle,         MeshMaterialEnum::Line    },
    { MeshEnum::Sword1_Grib,    MeshMaterialEnum::Default },
    { MeshEnum::Sword1_Blade,   MeshMaterialEnum::Metal   },
    { MeshEnum::Sword2_Grib,    MeshMaterialEnum::Default },
    { MeshEnum::Sword2_Blade,   MeshMaterialEnum::Metal   },
    { MeshEnum::Grass,          MeshMaterialEnum::Foliage },
};

}//NS