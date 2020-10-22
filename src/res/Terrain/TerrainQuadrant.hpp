//https://github.com/awwdev

#pragma once
#include "res/Terrain/TerrainMesh.hpp"
#include "gpu/RenderData/RenderData_General.hpp"
#include "com/box/Optional.hpp"
#include "com/box/Array.hpp"
#include "com/utils/Utils.hpp"

namespace rpg::res2 {

template<auto QUAD_COUNT, auto QUAD_SIZE>
struct Quadrant
{   
    //? meta
    static constexpr float QUADRANT_SIZE = QUAD_SIZE * QUAD_COUNT;
    using Vertex = gpu::RenderData_General::Vertex;

    //? data
    TerrainMeshIndexed<Vertex, QUAD_COUNT> mesh;


    void Create(float const quadrantIdx_z, float const quadrantIdx_x, idx_t const quadrantIdx, 
        com::Vec4f const& color = { 0.1f, 0.7f, 0.1f, 1 })
    {
        float const z = quadrantIdx_z * QUADRANT_SIZE;
        float const x = quadrantIdx_x * QUADRANT_SIZE;
        idx_t const i = quadrantIdx   * (QUAD_COUNT+1)*(QUAD_COUNT+1);
        mesh.Create(QUAD_SIZE, QUAD_SIZE, z, x, i, color);
    }

    com::Optional<com::Vec3f> RayIntersection(com::Ray const& ray) const
    {
        if (auto const aabb_intersection = RayAABB_Intersection(ray, mesh.aabb))
        {
            for(idx_t i = 0; i < mesh.INDEX_COUNT; i += 3) 
            {
                auto const& v0 = mesh.vertices[mesh.indices[i+0]].pos;
                auto const& v1 = mesh.vertices[mesh.indices[i+1]].pos;
                auto const& v2 = mesh.vertices[mesh.indices[i+2]].pos;
                
                if (auto const ray_intersection = RayTriangle_Intersection(ray, v0, v1, v2))
                    return ray_intersection;
            }
        }
        return {};
    }

};

}//ns