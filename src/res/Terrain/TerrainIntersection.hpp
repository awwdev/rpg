//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "com/Utils.hpp"
#include "com/box/Optional.hpp"
#include "res/Terrain/Terrain.hpp"

namespace rpg::res2 {

using Optional_VertexIndex = com::Optional<idx_t>;


template<auto QUAD_COUNT, auto QUAD_SIZE>
Optional_VertexIndex 
RayQuadrantIntersection
(com::Ray const& ray, Quadrant<QUAD_COUNT, QUAD_SIZE> const& quadrant)
{
    if (com::RayAABB_Intersection(ray, quadrant.aabb)) {
        //from the intersection point, we can limit the triangles to test, 
        //since they should be near intersection 
    }
    return {};
}


template<auto QUADRANT_COUNT_T, auto QUAD_COUNT, auto QUAD_SIZE>
Optional_VertexIndex 
RayTerrainIntersection
(com::Ray const& ray, Terrain<QUADRANT_COUNT_T, QUAD_COUNT, QUAD_SIZE> const& terrain)
{
    for(auto z = 0; z < terrain.QUADRANT_COUNT; ++z) {
    for(auto x = 0; x < terrain.QUADRANT_COUNT; ++x) {
        auto& quadrant = terrain.quadrants[z][x];
        if (auto const intersection = RayQuadrantIntersection(ray, quadrant))
            return intersection;
    }}
    return {};
}

} //ns