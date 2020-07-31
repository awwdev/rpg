//https://github.com/awwdev

#pragma once
#include "mini/Utils/Structs.hpp"
#include "mini/Box/Array.hpp"

#include "mini/Resources/Terrain/Terrain_Quadrant.hpp"
#include "mini/Resources/Terrain/Terrain_Serialization.hpp"

namespace mini::res2
{
    template<auto QUAD_COUNT, auto QUADRANT_LENGTH, auto QUADRANT_COUNT_T>
    struct Terrain
    {
        using QUADRANT_T = Quadrant<QUAD_COUNT, QUADRANT_LENGTH>;

        static constexpr auto QUADRANT_COUNT = QUADRANT_COUNT_T;
        QUADRANT_T quadrants [QUADRANT_COUNT][QUADRANT_COUNT];
        
        u32 editingQuadrant = 0;
        const QUADRANT_T& GetEditingQuadrant() const
        {       
            const auto x = editingQuadrant % QUADRANT_COUNT;
            const auto z = editingQuadrant / QUADRANT_COUNT;
            return quadrants[z][x];
        }

        void Create()
        {
            for(idx_t z = 0; z < QUADRANT_COUNT; ++z) {
            for(idx_t x = 0; x < QUADRANT_COUNT; ++x) {
                quadrants[z][x].Create((float)z * QUADRANT_LENGTH, (float)x * QUADRANT_LENGTH);
            }}
        }
    };

}//ns