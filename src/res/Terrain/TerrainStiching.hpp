//https://github.com/awwdev

#pragma once
#include "res/Terrain/Terrain.hpp"

namespace rpg::res {

inline void StichTerrain(auto& terrain, idx_t const quadrantId)
{
    auto& thisMesh = terrain.quadrants[quadrantId].mesh;
    auto const neighbours = terrain.GetQuadrantNeighbourBitset(quadrantId);

    auto const stichFn = [&](auto&... vertices)
    {
        com::Vec3f midPos {};
        ((midPos += vertices.pos), ...);
        midPos /= sizeof...(vertices);

        com::Vec4f midCol {};
        ((midCol += vertices.col), ...);
        midCol /= sizeof...(vertices);

        ((vertices.pos = midPos), ...);
        ((vertices.col = midCol), ...);
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

    if (neighbours == (QuadrantNorth | QuadrantEast)) 
    {
        auto& nighMeshNE = terrain.GetQuadrantNeighbour(quadrantId, QuadrantNorth | QuadrantEast).mesh;
        auto& nighMeshN  = terrain.GetQuadrantNeighbour(quadrantId, QuadrantNorth).mesh;
        auto& nighMeshE  = terrain.GetQuadrantNeighbour(quadrantId, QuadrantEast).mesh;

        auto& thisVertex   = thisMesh.vertices[VERTEX_COUNT_SIDE - 1];  
        auto& nighVertexNE = nighMeshNE.vertices[VERTEX_COUNT_TOTAL - VERTEX_COUNT_SIDE];
        auto& nighVertexN  = nighMeshN.vertices[VERTEX_COUNT_TOTAL - 1];
        auto& nighVertexE  = nighMeshE.vertices[0];

        stichFn(thisVertex, nighVertexNE, nighVertexN, nighVertexE);
        nighMeshNE.Recalculate();
        nighMeshN.Recalculate();
        nighMeshE.Recalculate();
    }

    if (neighbours == (QuadrantNorth | QuadrantWest)) 
    {
        auto& nighMeshNW = terrain.GetQuadrantNeighbour(quadrantId, QuadrantNorth | QuadrantWest).mesh;
        auto& nighMeshN  = terrain.GetQuadrantNeighbour(quadrantId, QuadrantNorth).mesh;
        auto& nighMeshW  = terrain.GetQuadrantNeighbour(quadrantId, QuadrantWest).mesh;

        auto& thisVertex   = thisMesh.vertices[0];  
        auto& nighVertexNW = nighMeshNW.vertices[VERTEX_COUNT_TOTAL - 1];
        auto& nighVertexN  = nighMeshN.vertices[VERTEX_COUNT_TOTAL - VERTEX_COUNT_SIDE];
        auto& nighVertexW  = nighMeshW.vertices[VERTEX_COUNT_SIDE - 1];

        stichFn(thisVertex, nighVertexNW, nighVertexN, nighVertexW);
        nighMeshNW.Recalculate();
        nighMeshN.Recalculate();
        nighMeshW.Recalculate();
    }

    if (neighbours == (QuadrantSouth | QuadrantEast)) 
    {
        auto& nighMeshSE = terrain.GetQuadrantNeighbour(quadrantId, QuadrantSouth | QuadrantEast).mesh;
        auto& nighMeshS  = terrain.GetQuadrantNeighbour(quadrantId, QuadrantSouth).mesh;
        auto& nighMeshE  = terrain.GetQuadrantNeighbour(quadrantId, QuadrantEast).mesh;

        auto& thisVertex   = thisMesh.vertices[VERTEX_COUNT_TOTAL - 1];  
        auto& nighVertexSE = nighMeshSE.vertices[0];
        auto& nighVertexS  = nighMeshS.vertices[VERTEX_COUNT_SIDE - 1];
        auto& nighVertexE  = nighMeshE.vertices[VERTEX_COUNT_TOTAL - VERTEX_COUNT_SIDE];

        stichFn(thisVertex, nighVertexSE, nighVertexS, nighVertexE);
        nighMeshSE.Recalculate();
        nighMeshS.Recalculate();
        nighMeshE.Recalculate();
    }

    if (neighbours == (QuadrantSouth | QuadrantWest)) 
    {
        auto& nighMeshSW = terrain.GetQuadrantNeighbour(quadrantId, QuadrantSouth | QuadrantWest).mesh;
        auto& nighMeshS  = terrain.GetQuadrantNeighbour(quadrantId, QuadrantSouth).mesh;
        auto& nighMeshW  = terrain.GetQuadrantNeighbour(quadrantId, QuadrantWest).mesh;

        auto& thisVertex   = thisMesh.vertices[VERTEX_COUNT_TOTAL - VERTEX_COUNT_SIDE];  
        auto& nighVertexSW = nighMeshSW.vertices[VERTEX_COUNT_SIDE - 1];
        auto& nighVertexS  = nighMeshS.vertices[0];
        auto& nighVertexW  = nighMeshW.vertices[VERTEX_COUNT_TOTAL - 1];

        stichFn(thisVertex, nighVertexSW, nighVertexS, nighVertexW);
        nighMeshSW.Recalculate();
        nighMeshS.Recalculate();
        nighMeshW.Recalculate();
    }

    thisMesh.Recalculate();
}

}//ns