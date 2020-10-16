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

    VertexBuffer<GeneralVertex, RD::TERRA_VERT_MAX_ALL> vboTerrain;

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
        //TODO: all quadrants
        const auto& verts    = resTerrain.terrain.quadrants[0][0].verts;
        const auto vertCount = resTerrain.terrain.quadrants[0][0].VERT_COUNT_TOTAL;
        vboTerrain.Append(verts, vertCount);

        //renderData.debugInfo.vboData_general_vertCount = vertCount;
    }

    void Create(VkCommandPool cmdPool, const res::Resources_Meshes& resMeshes)
    {
        vboTerrain.Create();

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