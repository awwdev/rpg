//https://github.com/awwdev

#pragma once
#include "res/Terrain/TerrainQuadrant.hpp"
#include "com/box/optional.hpp"
#include "com/utils/Utils.hpp"

namespace rpg::res {

struct RayTerrain_Intersection
{
    idx_t quadrantIdx;
    com::Vec3f point;
    uint32_t quadrantClosestVertexIndex; //not an absolute vertex index (of all terrain vertices)
};

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
            quadrants[i].Create(z, x, i);
            //{ z / (float)QUADRANT_COUNT, x / (float)QUADRANT_COUNT, 1, 1} );
        }
    }

    com::Optional<RayTerrain_Intersection> 
    RayIntersection(com::Ray const& ray) const
    {
        FOR_C_ARRAY(quadrants, i) {
            auto const& quadrant = quadrants[i];
            if (auto const intersection = quadrant.RayIntersection(ray))
                return { i, intersection->point, intersection->closestVertexIndex };
        }
        return {};
    }

    auto& GetVertexByIntersection(RayTerrain_Intersection const& intersection) 
    {
        auto const& quadrantIdx = intersection.quadrantIdx;
        auto const& vertexIdx = intersection.quadrantClosestVertexIndex;
        auto& vertex = quadrants[quadrantIdx].mesh.vertices[vertexIdx];
        return vertex;
    }

};

} //ns