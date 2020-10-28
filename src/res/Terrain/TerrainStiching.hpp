//https://github.com/awwdev

#pragma once
#include "res/Terrain/Terrain.hpp"

namespace rpg::res {

inline void StichTerrain(auto& terrain, idx_t const quadrantId)
{
    auto& mesh = terrain.quadrants[quadrantId].mesh;
    auto& vertices = mesh.vertices;
    auto const neighbours = terrain.GetQuadrantNeighbourBitset(quadrantId);

    auto const stichFn = [&](TerrainVertex& v1, TerrainVertex& v2)
    {
        auto const midPos = (v1.pos + v2.pos) * 0.5;
        auto const midCol = (v1.col + v2.col) * 0.5;
        v1.pos = v2.pos = midPos;
        v1.col = v2.col = midCol;
    };

    //? stich per neighbour

    if (neighbours & TerrainNorth) 
    {
        auto& neighbourQuadrant = terrain.GetQuadrantNeighbour(quadrantId, TerrainNorth);
        auto& neighbourVertices = neighbourQuadrant.mesh.vertices;
        for(idx_t i = 0; i < mesh.VERTEX_COUNT_ROW; ++i)
        {
            auto& vertex = vertices[i];
            auto& neighbourVertex = neighbourVertices[mesh.VERTEX_COUNT_TOTAL - mesh.VERTEX_COUNT_ROW + i];
            stichFn(vertex, neighbourVertex);
        }
        neighbourQuadrant.mesh.Recalculate();
    }

    if (neighbours & TerrainSouth) 
    {
        auto& neighbourQuadrant = terrain.GetQuadrantNeighbour(quadrantId, TerrainSouth);
        auto& neighbourVertices = neighbourQuadrant.mesh.vertices;
        for(idx_t i = 0; i < mesh.VERTEX_COUNT_ROW; ++i)
        {
            auto& vertex = vertices[mesh.VERTEX_COUNT_TOTAL - mesh.VERTEX_COUNT_ROW + i];
            auto& neighbourVertex = neighbourVertices[i];
            stichFn(vertex, neighbourVertex);
        }
        neighbourQuadrant.mesh.Recalculate();
    }

    /*
    if (neighbours & East)  
    {
        auto& neighbourVertices = GetQuadrantNeighbour(quadrantId, East).mesh.vertices;

        dbg::LogInfo("East");
    }

    if (neighbours & West) 
    {
        auto& neighbourVertices = GetQuadrantNeighbour(quadrantId, West).mesh.vertices;

        dbg::LogInfo("West");
    } 

    if (neighbours == (North | East)) 
    {
        auto& neighbourVertices = GetQuadrantNeighbour(quadrantId, North | East).mesh.vertices;

        dbg::LogInfo("North | East");
    }

    if (neighbours == (North | West))
    {
        auto& neighbourVertices = GetQuadrantNeighbour(quadrantId, North | West).mesh.vertices;

        dbg::LogInfo("North | West");
    } 

    if (neighbours == (South | East)) 
    {
        auto& neighbourVertices = GetQuadrantNeighbour(quadrantId, South | East).mesh.vertices;

        dbg::LogInfo("South | East");
    }
    
    if (neighbours == (South | West)) 
    {
        auto& neighbourVertices = GetQuadrantNeighbour(quadrantId, South | West).mesh.vertices;

        dbg::LogInfo("South | West");
    }
    */

    mesh.Recalculate();
}

}//ns