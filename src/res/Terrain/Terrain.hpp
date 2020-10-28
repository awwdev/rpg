//https://github.com/awwdev

#pragma once
#include "res/Terrain/TerrainQuadrant.hpp"
#include "com/box/optional.hpp"
#include "com/utils/Utils.hpp"
#include "com/box/SimpleArray.hpp"

namespace rpg::res {

template<auto QUADRANT_COUNT_T, auto QUAD_COUNT, auto QUAD_SIZE>
struct Terrain
{
    //? meta
    static constexpr auto QUADRANT_COUNT_ROW = QUADRANT_COUNT_T;
    using QUADRANT_T = Quadrant<QUAD_COUNT, QUAD_SIZE>;

    //? data
    QUADRANT_T quadrants [QUADRANT_COUNT_ROW * QUADRANT_COUNT_ROW];

    void Create()
    {
        FOR_C_ARRAY(quadrants, i) {
            auto const z = i / QUADRANT_COUNT_ROW;
            auto const x = i % QUADRANT_COUNT_ROW;
            quadrants[i].Create(z, x, i);
            //{ z / (float)QUADRANT_COUNT, x / (float)QUADRANT_COUNT, 1, 1} );
        }
    }

    com::Optional<RayQuadrant_Intersection> 
    RayIntersection(com::Ray const& ray) const
    {
        FOR_C_ARRAY(quadrants, i) {
            auto const& quadrant = quadrants[i];
            if (auto const intersection = quadrant.RayIntersection(ray))
                return intersection;
        }
        return {};
    }

    auto& GetVertex(idx_t const quadrantIdx, idx_t const vertexIdx)
    {
        auto& quadrant = quadrants[quadrantIdx];
        auto& meshVertices = quadrant.mesh.vertices;
        return meshVertices[vertexIdx];
    }

    void Stich(idx_t const quadrantIdx)
    {
        auto const x = quadrantIdx % QUADRANT_COUNT_ROW;
        auto const z = quadrantIdx / QUADRANT_COUNT_ROW;

        enum
        { 
            North  = 0b0001, 
            East   = 0b0010, 
            West   = 0b0100, 
            South  = 0b1000, 
        };
        int neighbours = 0;

        if (x < QUADRANT_COUNT_ROW - 1) neighbours |= East;
        if (z < QUADRANT_COUNT_ROW - 1) neighbours |= South;
        if (x > 0) neighbours |= West;
        if (z > 0) neighbours |= North;

        if (neighbours & North) 
        {
            idx_t const otherQuadrantIdx = quadrantIdx - QUADRANT_COUNT_ROW;
            dbg::LogInfo("North");
            dbg::LogInfo(otherQuadrantIdx);
        }

        if (neighbours & East)  
        {
            idx_t const otherQuadrantIdx = quadrantIdx + 1;
            dbg::LogInfo("East");
            dbg::LogInfo(otherQuadrantIdx);
        }

        if (neighbours & West) 
        {
            idx_t const otherQuadrantIdx = quadrantIdx - 1;
            dbg::LogInfo("West");
            dbg::LogInfo(otherQuadrantIdx);
        } 

        if (neighbours & South) 
        {
            idx_t const otherQuadrantIdx = quadrantIdx + QUADRANT_COUNT_ROW;
            dbg::LogInfo("South");
            dbg::LogInfo(otherQuadrantIdx);
        }

        if (neighbours == (North | East)) 
        {
            idx_t const otherQuadrantIdx = quadrantIdx - QUADRANT_COUNT_ROW + 1;
            dbg::LogInfo("North | East");
            dbg::LogInfo(otherQuadrantIdx);
        }

        if (neighbours == (North | West))
        {
            idx_t const otherQuadrantIdx = quadrantIdx - QUADRANT_COUNT_ROW - 1;
            dbg::LogInfo("North | West");
            dbg::LogInfo(otherQuadrantIdx);
        } 

        if (neighbours == (South | East)) 
        {
            idx_t const otherQuadrantIdx = quadrantIdx + QUADRANT_COUNT_ROW + 1;
            dbg::LogInfo("South | East");
            dbg::LogInfo(otherQuadrantIdx);
        }
        
        if (neighbours == (South | West)) 
        {
            idx_t const otherQuadrantIdx = quadrantIdx + QUADRANT_COUNT_ROW - 1;
            dbg::LogInfo("South | West");
            dbg::LogInfo(otherQuadrantIdx);
        }

    }

};

} //ns