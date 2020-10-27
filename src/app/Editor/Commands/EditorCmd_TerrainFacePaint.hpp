//https://github.com/awwdev

#pragma once
#include "ecs/ECS.hpp"
#include "res/Resources.hpp"
#include "app/Editor/EditorBrush.hpp"
#include "app/Editor/EditorCommandDirection.hpp"

namespace rpg::app {

struct EditorCmd_TerrainFacePaint
{
    com::Vec4f* triangleColor;
    com::Vec4f  brushColor;
    com::Vec4f  triangleColorReversed;

    EditorCmd_TerrainFacePaint(com::Vec4f& pTriangleColor, com::Vec4f const& pBrushColor)
        : triangleColor { &pTriangleColor }
        , brushColor    { pBrushColor }
    {}

    /*
    void Execute(res::Resources&, ecs::ECS&)
    {
        triangleColorReversed = *triangleColor;
        *triangleColor = brushColor;
    }
    
    void ExecuteReverse(res::Resources&, ecs::ECS&)
    {
        *triangleColor = triangleColorReversed;
    }
    */

    template<EditorCommandDirection DIR>
    void Execute(res::Resources& res, ecs::ECS& ecs)
    {
    }
};

} //ns