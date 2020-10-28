//https://github.com/awwdev

#pragma once
#include "res/Terrain/TerrainQuadrant.hpp"
#include "com/box/optional.hpp"
#include "com/utils/Utils.hpp"
#include "com/box/SimpleArray.hpp"

namespace rpg::res {

enum 
{ 
    QuadrantNorth = 0b0001, 
    QuadrantEast  = 0b0010, 
    QuadrantWest  = 0b0100, 
    QuadrantSouth = 0b1000,
};

template<idx_t QUADRANT_COUNT_T, idx_t QUAD_COUNT, idx_t QUAD_SIZE>
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

    auto& GetVertex(idx_t const quadrantId, idx_t const vertexId)
    {
        return quadrants[quadrantId].mesh.vertices[vertexId];
    }

    auto& GetTriangleColor(idx_t const quadrantId, idx_t const triangleId)
    {
        return quadrants[quadrantId].mesh.triangleColors[triangleId];
    }

    auto GetQuadrantNeighbourBitset(idx_t const quadrantId) const 
    {
        int neighbourBitset = 0;
        auto const x = quadrantId % QUADRANT_COUNT_ROW;
        auto const z = quadrantId / QUADRANT_COUNT_ROW;
        if (x < QUADRANT_COUNT_ROW - 1) neighbourBitset |= QuadrantEast;
        if (z < QUADRANT_COUNT_ROW - 1) neighbourBitset |= QuadrantSouth; 
        if (x > 0) neighbourBitset |= QuadrantWest;
        if (z > 0) neighbourBitset |= QuadrantNorth;
        return neighbourBitset;
    }

    auto& GetQuadrantNeighbour(idx_t const quadrantId, int dir)
    {
        switch(dir)
        {
            case QuadrantNorth: return quadrants[quadrantId - QUADRANT_COUNT_ROW];
            case QuadrantSouth: return quadrants[quadrantId + QUADRANT_COUNT_ROW];
            case QuadrantWest:  return quadrants[quadrantId - 1];
            case QuadrantEast:  return quadrants[quadrantId + 1];
            case (QuadrantNorth | QuadrantEast): return quadrants[quadrantId - QUADRANT_COUNT_ROW + 1];
            case (QuadrantNorth | QuadrantWest): return quadrants[quadrantId - QUADRANT_COUNT_ROW - 1];
            case (QuadrantSouth | QuadrantEast): return quadrants[quadrantId + QUADRANT_COUNT_ROW + 1];
            case (QuadrantSouth | QuadrantWest): return quadrants[quadrantId + QUADRANT_COUNT_ROW - 1];

            default: dbg::Assert(false, "cannot return neighbout"); return quadrants[0];
        }
    }

};

} //ns