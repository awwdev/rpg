//https://github.com/awwdev

#pragma once
#include "mini/Utils/Structs.hpp"
#include "mini/Box/Array.hpp"

namespace mini::res2
{
    template<auto QUAD_COUNT_T, auto QUADRANT_LENGTH_T>
    struct Quadrant
    {
        static constexpr auto LENGTH = QUADRANT_LENGTH_T; 

        static constexpr auto QUAD_COUNT       = QUAD_COUNT_T; 
        static constexpr auto QUAD_LENGTH      = LENGTH / (float)QUAD_COUNT_T;
        static constexpr auto CORNER_COUNT     = QUAD_COUNT + 1;
        static constexpr auto QUAD_COUNT_TOTAL = QUAD_COUNT * QUAD_COUNT;
        static constexpr auto VERT_COUNT_TOTAL = QUAD_COUNT_TOTAL * 6;

        float quadrantX;
        float quadrantZ;

        utils::Common_Vertex verts   [VERT_COUNT_TOTAL];
        box::Array<u32, 6>   corners [CORNER_COUNT][CORNER_COUNT];

        void Create()
        {
            constexpr math::Vec4f NORMAL_UP { 0, -1, 0, 1 };
            constexpr math::Vec4f COLOR     { 0.1f, 0.7f, 0.1f, 1 };

            //? CREATE VERTICES
            for(idx_t z = 0; z < QUAD_COUNT; ++z) {
            for(idx_t x = 0; x < QUAD_COUNT; ++x) {
                const auto v  = (z * QUAD_COUNT + x) * 6;
                const auto qX = quadrantX + x * QUAD_LENGTH;
                const auto qZ = quadrantZ + z * QUAD_LENGTH;
                verts[v+0] = { {  qX + 0.0f * QUAD_LENGTH, 0, qZ + 0.0f * QUAD_LENGTH, 1 }, NORMAL_UP, COLOR };
                verts[v+1] = { {  qX + 1.0f * QUAD_LENGTH, 0, qZ + 0.0f * QUAD_LENGTH, 1 }, NORMAL_UP, COLOR };
                verts[v+2] = { {  qX + 1.0f * QUAD_LENGTH, 0, qZ + 1.0f * QUAD_LENGTH, 1 }, NORMAL_UP, COLOR };
                verts[v+3] = verts[v+0];
                verts[v+4] = verts[v+2];
                verts[v+5] = { {  qX + 0.0f * QUAD_LENGTH, 0, qZ + 1.0f * QUAD_LENGTH, 1 }, NORMAL_UP, COLOR };
            }}

            //? CREATE CORNER DATA
            FOR_CARRAY(verts, i) {
                const auto vIdx = i % 6;
                const auto qIdx = i / 6; //quad
                switch(vIdx)
                {
                    case 0:
                    case 3:
                        corners[0][0].Append(vIdx);
                    break;

                    case 1:
                        corners[0][0].Append(vIdx);
                    break;

                    case 2:
                    case 4:
                        corners[0][0].Append(vIdx);
                    break;
                    
                    case 5:
                        corners[0][0].Append(vIdx);
                    break;
                }
            }

        }
    };

    template<auto QUAD_COUNT, auto QUADRANT_LENGTH, auto QUADRANT_COUNT_T>
    struct Terrain
    {
        static constexpr auto QUADRANT_COUNT = QUADRANT_COUNT_T;
        Quadrant<QUAD_COUNT, QUADRANT_LENGTH> quadrants [QUADRANT_COUNT][QUADRANT_COUNT];

        void Create()
        {
            for(idx_t z = 0; z < QUADRANT_COUNT; ++z) {
            for(idx_t x = 0; x < QUADRANT_COUNT; ++x) {
                quadrants[z][x].Create();
            }}
        }
    };

}//ns