//https://github.com/awwdev

#pragma once
#include "ecs/ECS.hpp"
#include "res/Resources.hpp"
#include "app/Editor/EditorBrush.hpp"
#include "app/Editor/EditorCommandDirection.hpp"

namespace rpg::app {

struct EditorCmd_TerrainVertexPaint
{
    

    template<EditorCommandDirection DIR>
    void Execute(res::Resources& res, ecs::ECS& ecs)
    {
        
    }

    /*
    void Execute(res::Resources&, ecs::ECS&)
    {
        FOR_SIMPLE_ARRAY(brushVertices, i)
        {
            auto const& vertexWeight = brushVertices[i].weight;
            auto& vertex = *brushVertices[i].vertexPtr;
            verticesReversed.Append(vertex);
            auto const colorBlended = com::InterpolateColors(vertex.col, brushColor, vertexWeight);
            vertex.col = colorBlended;
        }
    }
    */
    
    /*
    void ExecuteReverse(res::Resources&, ecs::ECS&)
    {
        FOR_SIMPLE_ARRAY(brushVertices, i)
        {
            auto const& vertexWeight = brushVertices[i].weight;
            auto& vertex = *brushVertices[i].vertexPtr;
            auto const& prevVertex = verticesReversed[i];
            vertex.col = prevVertex.col;
        }
    }
    */

   /*
   //TODO: probably a way to just store vertex pointers and initial and end state )also not whole vertex needed)
    EditorBrush::BrushVertices brushVertices;
    com::Vec4f brushColor;
    com::SimpleArray<res::TerrainVertex, 100> verticesReversed; 

    EditorCmd_TerrainVertexPaint(EditorBrush::BrushVertices const& pBrushVertices, com::Vec4f pBrushColor)
        : brushVertices { pBrushVertices }
        , brushColor    { pBrushColor }
    {}
    */

};

} //ns