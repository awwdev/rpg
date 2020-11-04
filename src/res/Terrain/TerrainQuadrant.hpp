//https://github.com/awwdev

#pragma once
#include "res/Terrain/TerrainMesh.hpp"
#include "gpu/RenderData/RenderData_Main.hpp"
#include "com/box/Optional.hpp"
#include "com/box/Array.hpp"
#include "com/utils/Utils.hpp"

namespace rpg::res {

struct RayQuadrant_Intersection
{
    com::Vec3f position;
    idx_t quadrantId;
    idx_t quadrantclosestVertexId;
    idx_t quadrantTriangleId;
    com::Vec4f interpolatedVertexColor;
};

template<idx_t QUAD_COUNT, idx_t QUAD_SIZE>
struct Quadrant
{   
    //? meta
    static constexpr float QUADRANT_SIZE = QUAD_SIZE * QUAD_COUNT;

    //? data
    TerrainMeshIndexed<QUAD_COUNT> mesh;
    idx_t quadrantId; //set by create (watch serialization)

    void Create(float const quadrantIdx_z, float const quadrantIdx_x, idx_t const pQuadrantIdx, 
        com::Vec4f const& color = { 0.1f, 0.7f, 0.1f, 1 })
    {
        quadrantId = pQuadrantIdx;
        float const z = quadrantIdx_z * QUADRANT_SIZE;
        float const x = quadrantIdx_x * QUADRANT_SIZE;
        idx_t const i = quadrantId * (QUAD_COUNT+1)*(QUAD_COUNT+1); //indices count
        mesh.Create(QUAD_SIZE, QUAD_SIZE, z, x, i, color);
    }

    com::Optional<RayQuadrant_Intersection> 
    RayIntersection(com::Ray const& ray) const
    {
        auto const aabbIntersection = RayAABB_Intersection(ray, mesh.aabb);
        if (!aabbIntersection) return {};

        for(idx_t i = 0; i < mesh.INDEX_COUNT; i += 3) 
        {
            //using indices to get triangles
            auto const& v0 = mesh.vertices[mesh.GetRelativeVertexIndex(i+0)];
            auto const& v1 = mesh.vertices[mesh.GetRelativeVertexIndex(i+1)];
            auto const& v2 = mesh.vertices[mesh.GetRelativeVertexIndex(i+2)];

            if (auto const triangleIntersection = RayTriangle_Intersection(ray, v0.pos, v1.pos, v2.pos))
            {
                com::Vec4f interpolatedVertexColor {};
                return { 
                    .position = triangleIntersection.Point(ray), 
                    .quadrantId = quadrantId, 
                    .quadrantclosestVertexId = (idx_t) mesh.absoluteIndices[i + triangleIntersection.GetClosestTriangleCorner()] - mesh.indicesOffset,
                    .quadrantTriangleId = i / 3,
                    .interpolatedVertexColor = triangleIntersection.GetInterpolatedVertexColor(v0.col, v1.col, v2.col)
                };
            }            
        }

        return {}; //hit aabb but no triangle
    }

};

}//ns