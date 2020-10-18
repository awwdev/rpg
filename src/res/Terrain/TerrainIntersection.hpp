//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "com/Utils.hpp"
#include "com/box/Optional.hpp"
#include "res/Terrain/Terrain.hpp"

namespace rpg::res2 {

using Optional_VertexIndex = com::Optional<idx_t>;


Optional_VertexIndex 
RayTriangleIntersection(auto& triangle, com::Ray<f32> const& ray)
{
    return {};
}


template<auto QUAD_COUNT, auto QUAD_SIZE>
Optional_VertexIndex 
RayQuadrantIntersection
(Quadrant<QUAD_COUNT, QUAD_SIZE> const& quadrant, com::Ray<f32> const& ray)
{
    auto const& aabb = quadrant.aabb;
    //TODO: get aabo from vertices
    //https://tavianator.com/2015/ray_box_nan.html
    return {};
}

template<auto QUADRANT_COUNT_T, auto QUAD_COUNT, auto QUAD_SIZE>
Optional_VertexIndex 
RayTerrainIntersection
(Terrain<QUADRANT_COUNT_T, QUAD_COUNT, QUAD_SIZE> const& terrain, com::Ray<f32> const& ray)
{
    for(auto z = 0; z < terrain.QUADRANT_COUNT; ++z) {
    for(auto x = 0; x < terrain.QUADRANT_COUNT; ++x) {
        auto& quadrant = terrain.quadrants[z][x];
        if (auto const intersection = RayQuadrantIntersection(quadrant, ray))
            return intersection;
    }}
    return {};
}

} //ns