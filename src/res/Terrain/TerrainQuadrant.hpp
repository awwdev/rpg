//https://github.com/awwdev

#pragma once

#include "com/box/Array.hpp"

namespace rpg::res {

template<auto QUAD_COUNT_T, auto QUADRANT_LEN_T>
struct Quadrant
{
    static constexpr auto QUADRANT_LEN = QUADRANT_LEN_T; 

    static constexpr auto QUAD_COUNT       = QUAD_COUNT_T; 
    static constexpr auto QUAD_LENGTH      = QUADRANT_LEN / (float)QUAD_COUNT_T;
    static constexpr auto CORNER_COUNT     = QUAD_COUNT + 1;
    static constexpr auto QUAD_COUNT_TOTAL = QUAD_COUNT * QUAD_COUNT;
    static constexpr auto VERT_COUNT_TOTAL = QUAD_COUNT_TOTAL * 6;

    float quadrantX;
    float quadrantZ;

    gpu::RenderData_General::Vertex verts     [VERT_COUNT_TOTAL];
    com::Array<idx_t, 6> corners [CORNER_COUNT][CORNER_COUNT];

    void Create(const float pZ, const float pX)
    {
        using namespace com;

        quadrantX = pX;
        quadrantZ = pZ;

        constexpr Vec3f NORMAL_UP { 0, -1, 0 };
        constexpr Vec4f COLOR     { 0.1f, 0.7f, 0.1f, 1 };

        //? CREATE VERTICES
        for(idx_t z = 0; z < QUAD_COUNT; ++z) {
        for(idx_t x = 0; x < QUAD_COUNT; ++x) {
            const auto v  = (z * QUAD_COUNT + x) * 6;
            const auto qX = quadrantX + x * QUAD_LENGTH;
            const auto qZ = quadrantZ + z * QUAD_LENGTH;
            verts[v+0] = { {  qX + 0.0f * QUAD_LENGTH, 0, qZ + 0.0f * QUAD_LENGTH }, NORMAL_UP, COLOR };
            verts[v+1] = { {  qX + 1.0f * QUAD_LENGTH, 0, qZ + 0.0f * QUAD_LENGTH }, NORMAL_UP, COLOR };
            verts[v+2] = { {  qX + 1.0f * QUAD_LENGTH, 0, qZ + 1.0f * QUAD_LENGTH }, NORMAL_UP, COLOR };
            verts[v+3] = verts[v+0];
            verts[v+4] = verts[v+2];
            verts[v+5] = { {  qX + 0.0f * QUAD_LENGTH, 0, qZ + 1.0f * QUAD_LENGTH }, NORMAL_UP, COLOR };
        }}

        //? CREATE CORNER DATA (for settings)
        FOR_C_ARRAY(verts, i) {
            const auto corner = GetCornerByVertex(i);
            corners[corner.x][corner.y].Append(i); //zx
        }

    }

    com::Vec2i GetCornerByVertex(const idx_t i) const
    {
        const auto vIdx = i % 6;
        const auto qIdx = i / 6; //quad
        const auto qX = qIdx % QUAD_COUNT;
        const auto qZ = qIdx / QUAD_COUNT;
        idx_t cX = qX; //top left corner q == c
        idx_t cZ = qZ;
        switch(vIdx)
        {
            case 0: case 3:              break;   
            case 1:         cX++;        break;
            case 2: case 4: cX++; cZ++;  break;
            case 5:               cZ++;  break;
        }
        return { cZ, cX };
    }

    com::Array<idx_t, 6> GetVerticesByCorner(const com::Vec2i& pair) const
    {
        using namespace com;
        return corners[pair.x][pair.y]; //zx
    }

    void RecalculateNormalsOfTriangle(const idx_t triangleIdx)
    {
        auto& v1 = verts[triangleIdx + 0];
        auto& v2 = verts[triangleIdx + 1];
        auto& v3 = verts[triangleIdx + 2];
        const auto a = v2.pos - v1.pos;
        const auto b = v3.pos - v1.pos;
        const auto norm = Normalize(Cross(a, b));
        v1.nor = v2.nor = v3.nor = norm;
    }

    void RecalculateNormals()
    {
        for(idx_t i = 0; i < VERT_COUNT_TOTAL; i+=3)
            RecalculateNormalsOfTriangle(i);
    }   

};

}//ns