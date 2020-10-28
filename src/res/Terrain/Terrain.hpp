//https://github.com/awwdev

#pragma once
#include "res/Terrain/TerrainQuadrant.hpp"
#include "com/box/optional.hpp"
#include "com/utils/Utils.hpp"
#include "com/box/SimpleArray.hpp"

namespace rpg::res {

enum 
{ 
    TerrainNorth = 0b0001, 
    TerrainEast  = 0b0010, 
    TerrainWest  = 0b0100, 
    TerrainSouth = 0b1000,
};

template<auto QUADRANT_COUNT_T, auto QUAD_COUNT, auto QUAD_SIZE>
struct Terrain
{
    static constexpr auto QUADRANT_COUNT_ROW = QUADRANT_COUNT_T;
    using QUADRANT_T = Quadrant<QUAD_COUNT, QUAD_SIZE>;
    QUADRANT_T quadrants [QUADRANT_COUNT_ROW * QUADRANT_COUNT_ROW];

    void Create()
    {
        FOR_C_ARRAY(quadrants, i) {
            auto const z = i / QUADRANT_COUNT_ROW;
            auto const x = i % QUADRANT_COUNT_ROW;
            quadrants[i].Create(z, x, i);
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

    auto GetQuadrantNeighbourBitset(idx_t const quadrantId) const 
    {
        int neighbourBitset = 0;
        auto const x = quadrantId % QUADRANT_COUNT_ROW;
        auto const z = quadrantId / QUADRANT_COUNT_ROW;
        if (x < QUADRANT_COUNT_ROW - 1) neighbourBitset |= TerrainEast;
        if (z < QUADRANT_COUNT_ROW - 1) neighbourBitset |= TerrainSouth; 
        if (x > 0) neighbourBitset |= TerrainWest;
        if (z > 0) neighbourBitset |= TerrainNorth;
        return neighbourBitset;
    }

    auto& GetQuadrantNeighbour(idx_t const quadrantId, int dir)
    {
        switch(dir)
        {
            case TerrainNorth: return quadrants[quadrantId - QUADRANT_COUNT_ROW];
            case TerrainSouth: return quadrants[quadrantId + QUADRANT_COUNT_ROW];
            case TerrainWest:  return quadrants[quadrantId - 1];
            case TerrainEast:  return quadrants[quadrantId + 1];
            case (TerrainNorth | TerrainEast): return quadrants[quadrantId - QUADRANT_COUNT_ROW + 1];
            case (TerrainNorth | TerrainWest): return quadrants[quadrantId - QUADRANT_COUNT_ROW - 1];
            case (TerrainSouth | TerrainEast): return quadrants[quadrantId + QUADRANT_COUNT_ROW + 1];
            case (TerrainSouth | TerrainWest): return quadrants[quadrantId + QUADRANT_COUNT_ROW - 1];

            default: dbg::Assert(false, "cannot return neighbout"); return quadrants[0];
        }
    }

};

} //ns