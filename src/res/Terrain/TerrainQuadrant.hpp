//https://github.com/awwdev

#pragma once
#include "res/Terrain/TerrainMesh.hpp"
#include "gpu/RenderData/RenderData_General.hpp"
#include "com/box/Optional.hpp"
#include "com/box/Array.hpp"
#include "com/utils/Utils.hpp"

namespace rpg::res {

struct RayQuadrant_Intersection
{
    com::Vec3f position;
    uint32_t quadrantId;
    //relative data
    uint32_t quadrantclosestVertexIdx;
    uint32_t quadrantTriangleIdx;
};

template<auto QUAD_COUNT, auto QUAD_SIZE>
struct Quadrant
{   
    //? meta
    static constexpr float QUADRANT_SIZE = QUAD_SIZE * QUAD_COUNT;

    //? data
    TerrainMeshIndexed<QUAD_COUNT> mesh;
    uint32_t quadrantIdx; //set by create (watch serialization)

    void Create(float const quadrantIdx_z, float const quadrantIdx_x, idx_t const pQuadrantIdx, 
        com::Vec4f const& color = { 0.1f, 0.7f, 0.1f, 1 })
    {
        quadrantIdx = pQuadrantIdx;
        float const z = quadrantIdx_z * QUADRANT_SIZE;
        float const x = quadrantIdx_x * QUADRANT_SIZE;
        idx_t const i = quadrantIdx * (QUAD_COUNT+1)*(QUAD_COUNT+1); //indices count
        mesh.Create(QUAD_SIZE, QUAD_SIZE, z, x, i, color);
    }

    com::Optional<RayQuadrant_Intersection> 
    RayIntersection(com::Ray const& ray) const
    {
        auto const aabbIntersection = RayAABB_Intersection(ray, mesh.aabb);
        if (!aabbIntersection) return {};

        for(uint32_t i = 0; i < mesh.INDEX_COUNT; i += 3) 
        {
            //using indices to get triangles
            auto const& v0 = mesh.vertices[mesh.GetRelativeVertexIndex(i+0)].pos;
            auto const& v1 = mesh.vertices[mesh.GetRelativeVertexIndex(i+1)].pos;
            auto const& v2 = mesh.vertices[mesh.GetRelativeVertexIndex(i+2)].pos;

            if (auto const triangleIntersection = RayTriangle_Intersection(ray, v0, v1, v2))
            {
                return { 
                    triangleIntersection.Point(ray), 
                    quadrantIdx, 
                    mesh.absoluteIndices[i + triangleIntersection.GetClosestTriangleCorner()] - mesh.indicesOffset,
                    i / 3,
                };
            }            
        }

        return {}; //hit aabb but no triangle
    }

};

}//ns