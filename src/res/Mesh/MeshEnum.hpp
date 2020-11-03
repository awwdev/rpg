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
    Cube,
    Grass,
    ENUM_END,
};

const com::EnumMap<MeshEnum::ENUM_END, com::String<PREFAB_FILE_LINE_LEN_MAX>> MESH_ENUM_TO_STR
{
    { MeshEnum::None,           "Mesh_None" },
    { MeshEnum::Grass,          "Mesh_Grass" },
    { MeshEnum::Cube,           "Mesh_Cube" },
};
const auto MESH_STR_TO_ENUM = com::StringMapFromEnumMap<MeshEnum, PREFAB_FILE_LINE_LEN_MAX>(MESH_ENUM_TO_STR);

inline com::EnumMap<MeshEnum::ENUM_END, com::String<PREFAB_FILE_LINE_LEN_MAX>> MESH_PATHS
{
    { MeshEnum::Grass,         "res/Meshes/grass.mesh"         },
    { MeshEnum::Cube,          "res/Meshes/cube.mesh"         },
};

}//NS