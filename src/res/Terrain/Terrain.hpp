//https://github.com/awwdev

#pragma once
#include "res/Terrain/TerrainQuadrant.hpp"
#include "com/Utils.hpp"

namespace rpg::res2 {

template<auto QUADRANT_COUNT_T, auto QUAD_COUNT, auto QUAD_SIZE>
struct Terrain
{
    //? meta
    static constexpr auto QUADRANT_COUNT = QUADRANT_COUNT_T;
    using QUADRANT_T = Quadrant<QUAD_COUNT, QUAD_SIZE>;

    //? data
    QUADRANT_T quadrants [QUADRANT_COUNT][QUADRANT_COUNT];

    void Create()
    {
        for(auto z = 0; z < QUADRANT_COUNT; ++z) {
        for(auto x = 0; x < QUADRANT_COUNT; ++x) {
            auto const quadrantIdx = x + z * QUADRANT_COUNT;
            quadrants[z][x].Create(z, x, quadrantIdx, 
            //test color
            { z/(float)QUADRANT_COUNT, x/(float)QUADRANT_COUNT, 1, 1});
        }}
    }

    auto& GetQuadrantByIndex(auto const idx) 
    {
        idx_t const z = idx / QUADRANT_COUNT;
        idx_t const x = idx % QUADRANT_COUNT;
        return quadrants[z][x];
    }

    auto RayIntersection(com::Ray const& ray)
    {
        for(auto z = 0; z < QUADRANT_COUNT; ++z) {
        for(auto x = 0; x < QUADRANT_COUNT; ++x) {
            auto& quadrant = terrain.quadrants[z][x];
            if (auto const intersection = quadrant.RayIntersection(ray, quadrant))
                return intersection;
        }}
        return {};
    }

};

} //ns