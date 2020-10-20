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

    auto RayIntersection(com::Ray const& ray) const -> com::Optional<com::Vec3f>
    {
        if (auto const aabb_intersection = RayAABB_Intersection(ray, mesh.aabb)) 
        {
            auto const distance = aabb_intersection.InnerDistance();
            auto const midpoint = aabb_intersection.MidPoint();

            FOR_C_ARRAY(mesh.vertices, i)
            {
                auto const& vertPos = mesh.vertices[i].pos;

                for (idx_t i = 0; i < mesh.INDEX_COUNT; i+=3)
                {
                    auto const& p0 = mesh.vertices[mesh.indices[i+0]].pos;
                    auto const& p1 = mesh.vertices[mesh.indices[i+1]].pos;
                    auto const& p2 = mesh.vertices[mesh.indices[i+2]].pos;
                    if (auto const triangle_intersection = com::RayTriangle_Intersection(ray, p0, p1, p2))
                    {

                    }
                }

            }

            return { aabb_intersection.EntryPoint() }; //currently the aabb pos (but not the real mesh pos)
        }       

        return {};
    }

};

}//ns