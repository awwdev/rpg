//https://github.com/awwdev

#pragma once
#include "com/box/EnumMap.hpp"
#include "com/box/String.hpp"

namespace rpg::app {

enum class EditorEnum : s32
{ 
    None,
    TerrainVertexMove,
    TerrainVertexPaint,
    TerrainFacePaint,
    PrefabPlacement,
    ENUM_END
};

struct EditorMode
{
    EditorEnum editorEnum = EditorEnum::PrefabPlacement;
    bool active = false;
};

com::EnumMap<EditorEnum::ENUM_END, com::String<50>> EDIT_MODE_ENUM_TO_STR 
{
    { EditorEnum::None,                 "None" },
    { EditorEnum::TerrainVertexMove,    "TerrainVertexMove" },
    { EditorEnum::TerrainVertexPaint,   "TerrainVertexPaint" },
    { EditorEnum::TerrainFacePaint,     "TerrainFacePaint" },
    { EditorEnum::PrefabPlacement,      "PrefabPlacement" },
};

}//ns