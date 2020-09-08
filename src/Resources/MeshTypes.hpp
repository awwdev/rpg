//https://github.com/awwdev

#pragma once
#include "box/StringMap.hpp"
#include "ECS/Components/Meta/ComponentParsing.hpp"

namespace rpg::res {

enum class MeshType
{
    PrimitiveCube,
    PrimitiveQuad,
    PrimitiveTriangle,
    PrimitiveRing16, 
    Sword,
    Grass,
    Stone,
    Tree,

    //could add Custom1 and so on for user mod mesh later
    ENUM_END
};

constexpr auto MESHTYPE_MAX_STR_LEN = 100;
const box::EnumMap<MeshType::ENUM_END, box::String<MESHTYPE_MAX_STR_LEN>> MESHTYPE_ENUM_TO_STR
{
    { MeshType::PrimitiveCube,      "PrimitiveCube" },
    { MeshType::PrimitiveQuad,      "PrimitiveQuad" },
    { MeshType::PrimitiveTriangle,  "PrimitiveTriangle" },
    { MeshType::PrimitiveRing16,    "PrimitiveRing16" },
    { MeshType::Sword,              "Sword" },
    { MeshType::Grass,              "Grass" },
    { MeshType::Stone,              "Stone" },
    { MeshType::Tree,               "Tree" },
};
const auto MESHTYPE_STR_TO_ENUM = box::StringMapFromEnumMap<MeshType, MESHTYPE_MAX_STR_LEN>(MESHTYPE_ENUM_TO_STR);

inline MeshType GetMeshType(const ecs::ComponentDataString& str)
{
    const MeshType* meshType = MESHTYPE_STR_TO_ENUM.GetOptional(str.data);
    dbg::Assert(meshType, "no mesh type found");
    return *meshType;
}

}//NS