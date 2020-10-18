//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "com/Utils.hpp"
#include "com/box/Optional.hpp"
#include "res/Terrain/Terrain.hpp"

namespace rpg::res2 {

using OptionalVertexIndex = com::Optional<idx_t>;


OptionalVertexIndex 
RayTriangleIntersection(auto& triangle, com::Ray<f32> const& ray)
{
    return {};
}


OptionalVertexIndex 
RayQuadrantIntersection(auto& quadrant, com::Ray<f32> const& ray)
{
    //TODO: get aabo from vertices
    //https://tavianator.com/2015/ray_box_nan.html
    return {};
}


OptionalVertexIndex 
RayTerrainIntersection(auto& terrain, com::Ray<f32> const& ray)
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