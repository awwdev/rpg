//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "com/Utils.hpp"
#include "com/Matrix.hpp"

namespace rpg::res2 {

struct Triangle 
{ 
    uint32_t   vertexIndices [3]; 
    com::Vec3f faceNormal;
}; 

template<typename VERTEX_T, auto QUAD_COUNT_T>
struct TerrainMeshIndexed
{
    //? meta
    static constexpr idx_t QUAD_COUNT       = QUAD_COUNT_T;
    static constexpr idx_t VERTEX_COUNT_ROW = QUAD_COUNT + 1;
    static constexpr idx_t VERTEX_COUNT     = VERTEX_COUNT_ROW * VERTEX_COUNT_ROW;
    static constexpr idx_t INDEX_COUNT      = QUAD_COUNT * QUAD_COUNT * 6;
    static constexpr idx_t TRIANGLE_COUNT   = QUAD_COUNT * QUAD_COUNT * 2;
    using Vertex = VERTEX_T;

    //? data
    Vertex    vertices  [VERTEX_COUNT];
    uint32_t  indices   [INDEX_COUNT];
    Triangle  triangles [TRIANGLE_COUNT];
    com::AABB aabb;

    //? triangle lookups
    idx_t vertexToTriangle   [VERTEX_COUNT];
    idx_t triangleToVertices [TRIANGLE_COUNT];


    void Create(
        float const size_z, float const size_x, 
        float const offset_z, float const offset_x,
        idx_t const offset_indices, com::Vec4f const& color)
    {
        //vertices
        FOR_C_ARRAY(vertices, i) 
        {
            const auto x = i % VERTEX_COUNT_ROW;
            const auto y = (f32)(rand() % 2); //testing aabb
            const auto z = i / VERTEX_COUNT_ROW;
            vertices[i] = 
            {
                .pos = 
                { 
                    offset_x + x * size_x, 
                    y, 
                    offset_z + z * size_z 
                },
                .nor = { 0, -1, 0 },
                .col = color,
            };
        }

        //indices
        for(idx_t i = 0; i < INDEX_COUNT; i+=6)
        {
            auto v = i / 6;
            auto n = v / (VERTEX_COUNT_ROW - 1);
            v += n;
            v += offset_indices;

            indices[i+0] = v;
            indices[i+1] = v + 1;
            indices[i+2] = v + VERTEX_COUNT_ROW + 1;
            indices[i+3] = v;
            indices[i+4] = v + VERTEX_COUNT_ROW + 1;
            indices[i+5] = v + VERTEX_COUNT_ROW;
        }

        
        //triangles
        for(idx_t i = 0; i < TRIANGLE_COUNT; i+=2)
        {
            uint32_t const n = i;// + i/(COUNT_X*2 - 1);
            triangles[i+0] = { n, n+6, n+5, {} };
            triangles[i+1] = { n, n+1, n+6, {} };
        }
        //TODO
        //triangle lookups

        Recalculate();
    }

    void Recalculate()
    {
        aabb = com::CalculateAABB(vertices, VERTEX_COUNT);
        //normals
    }

};

}//ns