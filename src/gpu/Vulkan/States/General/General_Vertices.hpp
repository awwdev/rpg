//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/Objects/BufferExt.hpp"
#include "gpu/Vulkan/Helper/Utils.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "com/box/Array.hpp"

#include "res/Resources_Meshes.hpp"
#include "res/Resources_Terrain.hpp"
#include "gpu/RenderData/RenderData.hpp"

namespace rpg::gpu::vuk {

struct General_Vertices
{
    using RD = RenderData_General;
    using GeneralVertex = RD::Vertex;

    VertexBuffer<GeneralVertex, RD::TERRA_VERTICES_MAX_ALL> vboTerrain;
    IndexBuffer<uint32_t, RD::TERRA_INDICES_MAX_ALL> iboTerrain;

    VertexBuffer<GeneralVertex, res::MESHES_VERTS_TOTAL> vboMeshes;
    res::MeshVertexRange vboMeshesVertexRanges [(idx_t) res::MESHES_TOTAL];

    VkDeviceSize offsets [1] = {};
    static constexpr VkVertexInputBindingDescription bindings []
    {
        {
            .binding    = 0,
            .stride     = sizeof(GeneralVertex),
            .inputRate  = VK_VERTEX_INPUT_RATE_VERTEX
        }
    };

    static constexpr VkVertexInputAttributeDescription attributes []
    {
        {
            .location   = 0,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32_SFLOAT,
            .offset     = offsetof(GeneralVertex, pos),
        },
        {
            .location   = 1,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32_SFLOAT,
            .offset     = offsetof(GeneralVertex, nor),
        },
        {
            .location   = 2,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset     = offsetof(GeneralVertex, col),
        },
        {
            .location   = 3,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32_SFLOAT,
            .offset     = offsetof(GeneralVertex, tex),
        }
    };

    void Update(gpu::RenderData_General& renderData, const res::Resources_Terrain& resTerrain)
    {
        vboTerrain.Reset();
        
        //? terrain vertices
        auto const& quadrants = resTerrain.terrain.quadrants;
        FOR_C_ARRAY(quadrants, i)
        {
            auto const& quadrant = quadrants[i];
            vboTerrain.Append(quadrant.mesh.vertices, quadrant.mesh.VERTEX_COUNT_TOTAL);
        }
    }

    void Create(VkCommandPool cmdPool, res::Resources_Meshes const& resMeshes, const res::Resources_Terrain& resTerrain)
    {
        //? terrain
        vboTerrain.Create();
        iboTerrain.Create();

        auto const& quadrants = resTerrain.terrain.quadrants;
        FOR_C_ARRAY(quadrants, i)
        {
            auto const& quadrant = quadrants[i];
            iboTerrain.Append(quadrant.mesh.indices, quadrant.mesh.INDEX_COUNT);
        }
        iboTerrain.Bake(cmdPool);

        //? meshes
        vboMeshes.Create();
        if (resMeshes.allVertices.Empty() == false) {
            vboMeshes.Append(resMeshes.allVertices);
            vboMeshes.Bake(cmdPool);
            FOR_C_ARRAY(resMeshes.meshVertexRanges, i)
                vboMeshesVertexRanges[i] = resMeshes.meshVertexRanges[i];
        }
    }

    void Destroy()
    {
        vboTerrain.Destroy();
        iboTerrain.Destroy();
        vboMeshes.Destroy();
        FOR_C_ARRAY(vboMeshesVertexRanges, i)
            vboMeshesVertexRanges[i] = {};
    }
    ~General_Vertices()
    {
        Destroy();
    }

};

}//ns