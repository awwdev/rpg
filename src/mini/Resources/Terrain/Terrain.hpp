//https://github.com/awwdev

#pragma once
#include "mini/Utils/Structs.hpp"
#include "mini/Box/Array.hpp"
#include "mini/ECS/ECS.hpp"
#include "mini/Rendering/Camera.hpp"
#include "mini/Resources/Terrain/Terrain_Quadrant.hpp"
#include "mini/Resources/Terrain/Terrain_Serialization.hpp"

namespace mini::res2
{
    template<auto QUAD_COUNT, auto QUADRANT_LENGTH, auto QUADRANT_COUNT_T>
    struct Terrain
    {
        using QUADRANT_T = Quadrant<QUAD_COUNT, QUADRANT_LENGTH>;

        static constexpr auto QUADRANT_COUNT = QUADRANT_COUNT_T;
        static constexpr auto TOTAL_LENGTH   = QUADRANT_COUNT * QUADRANT_LENGTH;
        QUADRANT_T quadrants [QUADRANT_COUNT][QUADRANT_COUNT];
        
        u32 editingQuadrantIdx = 0;
        const QUADRANT_T& GetEditingQuadrant() const
        {       
            const auto x = editingQuadrantIdx % QUADRANT_COUNT;
            const auto z = editingQuadrantIdx / QUADRANT_COUNT;
            return quadrants[z][x];
        }
        QUADRANT_T& GetEditingQuadrant()
        {       
            const auto x = editingQuadrantIdx % QUADRANT_COUNT;
            const auto z = editingQuadrantIdx / QUADRANT_COUNT;
            return quadrants[z][x];
        }

        void Create()
        {
            for(idx_t z = 0; z < QUADRANT_COUNT; ++z) {
            for(idx_t x = 0; x < QUADRANT_COUNT; ++x) {
                quadrants[z][x].Create(
                    (float)z * QUADRANT_LENGTH - TOTAL_LENGTH * 0.5f, 
                    (float)x * QUADRANT_LENGTH - TOTAL_LENGTH * 0.5f);
            }}
        }

        void Update(const double dt, const rendering::Camera& camera, ecs::ECS& ecs)
        {   
            auto& editingQuadrant = GetEditingQuadrant();
            const math::Vec4f col = { 0.7f, 0.2f, 0.2f, 1 };

            for(u8 z = 0; z < QUAD_COUNT; ++z) {
            for(u8 x = 0; x < QUAD_COUNT; ++x) { 
                const auto idx = (z * QUAD_COUNT + x) * 6;
                editingQuadrant.verts[idx + 0].col = col;
                editingQuadrant.verts[idx + 1].col = col;
                editingQuadrant.verts[idx + 2].col = col;
                editingQuadrant.verts[idx + 3].col = col;
                editingQuadrant.verts[idx + 4].col = col;
                editingQuadrant.verts[idx + 5].col = col;
            }}

        }
    };

}//ns