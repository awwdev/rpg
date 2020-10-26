//https://github.com/awwdev

#pragma once
#include "ecs/ECS.hpp"
#include "res/Resources.hpp"
#include "res/Resources_Terrain.hpp"
#include "wnd/WindowEvents.hpp"

namespace rpg::app {

struct EditorCmd_TerrainVertexMove
{
    EditorBrush::BrushVertices brushVertices;
    com::SimpleArray<com::Vec3f, 100> posInitial; 
    com::SimpleArray<com::Vec3f, 100> posEnd; 

    decltype(res::Resources_Terrain::TERRAIN_T::QUADRANT_T::mesh)* meshPtr;

    void Execute(res::Resources&, ecs::ECS&)
    {
        FOR_SIMPLE_ARRAY(brushVertices, i)
        {
            auto& vertex = *brushVertices[i].vertexPtr;
            vertex.pos = posEnd[i];
        }
        meshPtr->Recalculate();
    }
    
    void ExecuteReverse(res::Resources&, ecs::ECS&)
    {
        FOR_SIMPLE_ARRAY(brushVertices, i)
        {
            auto& vertex = *brushVertices[i].vertexPtr;
            vertex.pos = posInitial[i];
        }
        meshPtr->Recalculate();
    }

};

} //ns