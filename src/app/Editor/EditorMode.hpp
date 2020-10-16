//https://github.com/awwdev

#pragma once
#include "com/box/EnumMap.hpp"
#include "com/box/String.hpp"

namespace rpg::app {

enum class EditorMode
{ 
    None,
    TerrainVertexGrab,
    TerrainVertexPaint,
    PrefabPlacement,
    ENUM_END
};
com::EnumMap<EditorMode::ENUM_END, com::String<50>> EDIT_MODE_ENUM_TO_STR 
{
    { EditorMode::None, "None" },
    { EditorMode::TerrainVertexGrab, "TerrainVertexGrab" },
    { EditorMode::TerrainVertexPaint, "TerrainVertexPaint" },
    { EditorMode::PrefabPlacement, "PrefabPlacement" },
};


}//ns