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

    auto& GetVertexByIntersection(RayQuadrant_Intersection const& intersection) 
    {
        auto const& quadrantIdx = intersection.quadrantIdx;
        auto const& vertexIdx = intersection.quadrantclosestVertexIdx;
        auto& vertex = quadrants[quadrantIdx].mesh.vertices[vertexIdx];
        return vertex;
    }

    template<auto N>
    void Stich(idx_t const quadrantIdx, com::SimpleArray<idx_t, N> const& verticesToCheck)
    {

    }

};

} //ns