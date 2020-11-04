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

struct Main_Vertices
{
    using RD = RenderData_Main;
    using GeneralVertex = RD::Vertex;

    VertexBuffer<GeneralVertex, RD::TERRA_VERTICES_MAX_ALL> vboTerrain;
    IndexBuffer<uint32_t, RD::TERRA_INDICES_MAX_ALL> iboTerrain;

    VertexBuffer<GeneralVertex, res::MESHES_VERTICES_TOTAL> vboMeshes;
    IndexBuffer<uint32_t, res::MESHES_INDICES_TOTAL> iboMeshes;

    res::VertexRanges vboMeshesVertexRanges;
    res::IndexRanges  iboMeshesIndexRanges;

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

    void Update(gpu::RenderData_Main& renderData, const res::Resources_Terrain& resTerrain)
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
            iboTerrain.Append(quadrant.mesh.absoluteIndices, quadrant.mesh.INDEX_COUNT);
        }
        iboTerrain.Bake(cmdPool);

        //? meshes
        vboMeshes.Create();
        iboMeshes.Create();
        //! shall not be empty
        vboMeshes.Append(resMeshes.allVertices);
        vboMeshes.Bake(cmdPool);
        iboMeshes.Append(resMeshes.allIndices);
        iboMeshes.Bake(cmdPool);
        FOR_C_ARRAY(resMeshes.vertexRanges, i)
            vboMeshesVertexRanges[i] = resMeshes.vertexRanges[i];
        FOR_C_ARRAY(resMeshes.indexRanges, i)
            iboMeshesIndexRanges[i] = resMeshes.indexRanges[i];
    }

    void Destroy()
    {
        vboTerrain.Destroy();
        iboTerrain.Destroy();
        vboMeshes.Destroy();
        iboMeshes.Destroy();
        FOR_C_ARRAY(vboMeshesVertexRanges, i)
            vboMeshesVertexRanges[i] = {};
        FOR_C_ARRAY(iboMeshesIndexRanges, i)
            iboMeshesIndexRanges[i] = {};
    }
    ~Main_Vertices()
    {
        Destroy();
    }

};

}//ns