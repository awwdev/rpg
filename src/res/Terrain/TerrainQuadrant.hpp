//https://github.com/awwdev

#pragma once
#include "res/Terrain/TerrainMesh.hpp"
#include "gpu/RenderData/RenderData_General.hpp"
#include "com/box/Optional.hpp"
#include "com/box/Array.hpp"
#include "com/Utils.hpp"

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
        if (auto const intersection = RayAABB_Intersection(ray, mesh.aabb)) 
        {
            auto const distance = intersection.InnerDistance();
            auto const midpoint = intersection.MidPoint();

            FOR_C_ARRAY(mesh.vertices, i)
            {
                auto const& vertPos = mesh.vertices[i].pos;
                if (com::Distance(vertPos, midpoint) < distance) //sphere distance
                {
                    dbg::LogInfo("vert idx", i);
                    //TODO: triangle intersection
                }
            }

            return { intersection.EntryPoint() }; //currently the aabb pos (but not the real mesh pos)
        }       

        return {};
    }

};

}//ns