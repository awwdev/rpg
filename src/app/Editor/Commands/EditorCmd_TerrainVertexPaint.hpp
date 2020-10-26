//https://github.com/awwdev

#pragma once
#include "ecs/ECS.hpp"
#include "res/Resources.hpp"
#include "app/Editor/EditorBrush.hpp"

namespace rpg::app {

struct EditorCmd_TerrainVertexPaint
{
    com::SimpleArray<res::TerrainVertex, 100> verticesPrevious;
    EditorBrush::BrushVertices brushVertices;
    com::Vec4f brushColor;

    void Execute(res::Resources&, ecs::ECS&)
    {
        FOR_SIMPLE_ARRAY(brushVertices, i)
        {
            auto const& vertexWeight = brushVertices[i].weight;
            auto& vertex = *brushVertices[i].vertexPtr;
            verticesPrevious.Append(vertex);
            auto const colorBlended = com::InterpolateColors(vertex.col, brushColor, vertexWeight);
            vertex.col = colorBlended;
        }
    }
    
    void ExecuteReverse(res::Resources&, ecs::ECS&)
    {
        FOR_SIMPLE_ARRAY(brushVertices, i)
        {
            auto const& vertexWeight = brushVertices[i].weight;
            auto& vertex = *brushVertices[i].vertexPtr;
            auto const& prevVertex = verticesPrevious[i];
            vertex.col = prevVertex.col;
        }
    }
};

} //ns