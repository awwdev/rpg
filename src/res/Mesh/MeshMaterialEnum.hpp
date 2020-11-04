//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "com/box/StringMap.hpp"
#include "res/Prefab/PrefabMeta.hpp"

namespace rpg::res {

enum class MeshMaterialEnum : u8
{
    General,
    Foliage,
    Metallic,
    Line, 
    ENUM_END
};

const com::EnumMap<MeshMaterialEnum::ENUM_END, com::String<PREFAB_FILE_LINE_LEN_MAX>> MESH_MATERIAL_ENUM_TO_STR
{
    { MeshMaterialEnum::General,    "General" },
    { MeshMaterialEnum::Foliage,    "Foliage" },
    { MeshMaterialEnum::Metallic,   "Metallic" },
    { MeshMaterialEnum::Line,       "Line" },
};
const auto MESH_MATERIAL_STR_TO_ENUM = com::StringMapFromEnumMap<MeshMaterialEnum, PREFAB_FILE_LINE_LEN_MAX>(MESH_MATERIAL_ENUM_TO_STR);

}//NS