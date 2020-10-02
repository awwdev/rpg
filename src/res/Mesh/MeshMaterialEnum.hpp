//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "com/box/StringMap.hpp"
#include "res/Prefab/PrefabMeta.hpp"

namespace rpg::res {

enum class MeshMaterialEnum : u8
{
    Default,
    Metallic,
    Wooden,
    ENUM_END
};

const com::EnumMap<MeshMaterialEnum::ENUM_END, com::String<PREFAB_FILE_LINE_LEN_MAX>> MESH_MATERIAL_ENUM_TO_STR
{
    { MeshMaterialEnum::Default,    "Default" },
    { MeshMaterialEnum::Metallic,   "Metallic" },
    { MeshMaterialEnum::Wooden,     "Wooden" },
};
const auto MESH_MATERIAL_STR_TO_ENUM = com::StringMapFromEnumMap<MeshMaterialEnum, PREFAB_FILE_LINE_LEN_MAX>(MESH_MATERIAL_ENUM_TO_STR);

}//NS