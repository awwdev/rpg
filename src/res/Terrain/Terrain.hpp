//https://github.com/awwdev

#pragma once
#include "res/Terrain/TerrainQuadrant.hpp"
#include "com/box/optional.hpp"
#include "com/utils/Utils.hpp"

namespace rpg::res2 {

template<auto QUADRANT_COUNT_T, auto QUAD_COUNT, auto QUAD_SIZE>
struct Terrain
{
    //? meta
    static constexpr auto QUADRANT_COUNT = QUADRANT_COUNT_T;
    using QUADRANT_T = Quadrant<QUAD_COUNT, QUAD_SIZE>;

    //? data
    QUADRANT_T quadrants [QUADRANT_COUNT * QUADRANT_COUNT];

    void Create()
    {
        FOR_C_ARRAY(quadrants, i) {
            auto const z = i / QUADRANT_COUNT;
            auto const x = i % QUADRANT_COUNT;
            quadrants[i].Create(z, x, i, { z / (float)QUADRANT_COUNT, x / (float)QUADRANT_COUNT, 1, 1} );
        }
    }

    struct RayTerrain_Intersection
    {
        com::Vec3f point;
        idx_t quadrantIdx;
    };

    auto RayIntersection(com::Ray const& ray) const -> com::Optional<RayTerrain_Intersection>
    {
        FOR_C_ARRAY(quadrants, i) {
            auto const& quadrant = quadrants[i];
            if (auto const intersectionPoint = quadrant.RayIntersection(ray))
                return {  *intersectionPoint, i };
        }
        return {};
    }

};

} //ns