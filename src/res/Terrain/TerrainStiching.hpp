//https://github.com/awwdev

#pragma once
#include "res/Terrain/Terrain.hpp"

namespace rpg::res {

inline void StichTerrain(auto& terrain, idx_t const quadrantId)
{
    auto& thisMesh = terrain.quadrants[quadrantId].mesh;
    auto const neighbours = terrain.GetQuadrantNeighbourBitset(quadrantId);

    auto const stichFn = [&](TerrainVertex& v1, TerrainVertex& v2)
    {
        auto const midPos = (v1.pos + v2.pos) * 0.5;
        auto const midCol = (v1.col + v2.col) * 0.5;
        v1.pos = v2.pos = midPos;
        v1.col = v2.col = midCol;
    };

    //? stich per neighbour
    auto constexpr VERTEX_COUNT_SIDE  = thisMesh.VERTEX_COUNT_ROW;
    auto constexpr VERTEX_COUNT_TOTAL = thisMesh.VERTEX_COUNT_TOTAL;

    //? sides

    if (neighbours & QuadrantNorth) 
    {
        auto& nighMesh = terrain.GetQuadrantNeighbour(quadrantId, QuadrantNorth).mesh;
        for(idx_t i = 0; i < VERTEX_COUNT_SIDE; ++i)
        {
            auto& thisVertex = thisMesh.vertices[i];
            auto& nighVertex = nighMesh.vertices[VERTEX_COUNT_TOTAL - VERTEX_COUNT_SIDE + i];
            stichFn(thisVertex, nighVertex);
        }
        nighMesh.Recalculate();
    }

    if (neighbours & QuadrantSouth) 
    {
        auto& nighMesh = terrain.GetQuadrantNeighbour(quadrantId, QuadrantSouth).mesh;
        for(idx_t i = 0; i < VERTEX_COUNT_SIDE; ++i)
        {
            auto& thisVertex = thisMesh.vertices[VERTEX_COUNT_TOTAL - VERTEX_COUNT_SIDE + i];
            auto& nighVertex = nighMesh.vertices[i];
            stichFn(thisVertex, nighVertex);
        }
        nighMesh.Recalculate();
    }

    
    if (neighbours & QuadrantEast)  
    {
        auto& nighMesh = terrain.GetQuadrantNeighbour(quadrantId, QuadrantEast).mesh;
        for(idx_t i = 0; i < VERTEX_COUNT_SIDE; ++i)
        {
            auto& thisVertex = thisMesh.vertices[i * VERTEX_COUNT_SIDE + VERTEX_COUNT_SIDE - 1];
            auto& nighVertex = nighMesh.vertices[i * VERTEX_COUNT_SIDE];
            stichFn(thisVertex, nighVertex);
        }
        nighMesh.Recalculate();
    }

    if (neighbours & QuadrantWest) 
    {
        auto& nighMesh = terrain.GetQuadrantNeighbour(quadrantId, QuadrantWest).mesh;
        for(idx_t i = 0; i < VERTEX_COUNT_SIDE; ++i)
        {
            auto& thisVertex = thisMesh.vertices[i * VERTEX_COUNT_SIDE];  
            auto& nighVertex = nighMesh.vertices[i * VERTEX_COUNT_SIDE + VERTEX_COUNT_SIDE - 1];
            stichFn(thisVertex, nighVertex);
        }
        nighMesh.Recalculate();
    } 

    //? corners

    //TODO: get vertices of a quadrant corner
    //TODO: get vertices of a quadrant sides

    if (neighbours == (QuadrantNorth | QuadrantEast)) 
    {
        auto& nighMesh = terrain.GetQuadrantNeighbour(quadrantId, QuadrantNorth | QuadrantEast).mesh;
        auto& thisVertex = thisMesh.vertices[VERTEX_COUNT_SIDE - 1];  
        auto& nighVertex = nighMesh.vertices[VERTEX_COUNT_TOTAL - VERTEX_COUNT_SIDE];
        stichFn(thisVertex, nighVertex);
        nighMesh.Recalculate();
    }

    if (neighbours == (QuadrantSouth | QuadrantWest)) 
    {
        auto& nighMesh = terrain.GetQuadrantNeighbour(quadrantId, QuadrantSouth | QuadrantWest).mesh;
        auto& thisVertex = thisMesh.vertices[VERTEX_COUNT_TOTAL - VERTEX_COUNT_SIDE];  
        auto& nighVertex = nighMesh.vertices[VERTEX_COUNT_SIDE - 1];
        stichFn(thisVertex, nighVertex);
        nighMesh.Recalculate();
    }

    if (neighbours == (QuadrantNorth | QuadrantWest)) 
    {
        auto& nighMesh = terrain.GetQuadrantNeighbour(quadrantId, QuadrantNorth | QuadrantWest).mesh;
        auto& thisVertex = thisMesh.vertices[0];  
        auto& nighVertex = nighMesh.vertices[VERTEX_COUNT_TOTAL - 1];
        stichFn(thisVertex, nighVertex);
        nighMesh.Recalculate();
    }

    if (neighbours == (QuadrantSouth | QuadrantEast)) 
    {
        auto& nighMesh = terrain.GetQuadrantNeighbour(quadrantId, QuadrantSouth | QuadrantEast).mesh;
        auto& thisVertex = thisMesh.vertices[VERTEX_COUNT_TOTAL - 1];  
        auto& nighVertex = nighMesh.vertices[0];
        stichFn(thisVertex, nighVertex);
        nighMesh.Recalculate();
    }

    thisMesh.Recalculate();
}

}//ns