//https://github.com/awwdev

#pragma once
#include "res/Terrain/TerrainGrid.hpp"
#include "gpu/RenderData/RenderData_General.hpp"
#include "com/box/Optional.hpp"
#include "com/box/Array.hpp"
#include "com/Utils.hpp"

namespace rpg::res2 {

template<auto QUAD_COUNT, auto QUAD_SIZE>
struct Quadrant
{   
    //? meta
    static constexpr idx_t VERTEX_COUNT_ROW = QUAD_COUNT + 1;
    static constexpr idx_t VERTEX_COUNT_TOTAL = VERTEX_COUNT_ROW * VERTEX_COUNT_ROW;
    static constexpr idx_t INDEX_COUNT = QUAD_COUNT * QUAD_COUNT * 6;
    static constexpr float QUADRANT_SIZE = QUAD_SIZE * QUAD_COUNT;
    using Vertex = gpu::RenderData_General::Vertex;

    //? data
    GridMesh_Indexed<Vertex> gridMesh;
    idx_t quadrantIdx = 0; //assigned by create function

    void Create(float const qIndex_z, float const qIndex_x, idx_t const pQuadrantIdx, com::Vec4f const& color = { 0.1f, 0.7f, 0.1f, 1 })
    {
        quadrantIdx = pQuadrantIdx;
        float const offset_z = qIndex_z * QUADRANT_SIZE;
        float const offset_x = qIndex_x * QUADRANT_SIZE;
        idx_t const offset_v = quadrantIdx * VERTEX_COUNT_TOTAL; //vertex index offset (one holistic vbo)
        gridMesh.Create();
    }

    void UpdateMesh()
    {
        gridMesh.Recalculate();
    }

    auto RayIntersection(com::Ray const& ray) const -> com::Optional<com::Vec3f>
    {
        if (auto const intersection = RayAABB_Intersection(ray, gridMesh.aabb)) 
        {
            auto const distance = intersection.InnerDistance(ray);
            auto const midpoint = intersection.MidPoint(ray);

            for(auto i = 0; i < VERTEX_COUNT_TOTAL; ++i)
            {
                //if (com::Distance((GetVertices1DArray()[i]).pos, midpoint) < distance)
                {
                    //get all triangles by one vertex
                }
            }

            return { 0.f, 0.f, 0.f };

        }            
        return {};
    }

};

}//ns