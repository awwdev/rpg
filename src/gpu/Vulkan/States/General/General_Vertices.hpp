//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/Objects/BufferExt.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "gpu/RenderData/_Old/RenderStructs.hpp"
#include "com/box/Array.hpp"

#include "res/_Old/CpuResources.hpp"
#include "res/Resources_Models.hpp"
#include "gpu/RenderData/RenderData.hpp"

namespace rpg::gpu::vuk {

struct General_Vertices
{
    using RD = RenderData_General;
    using GeneralVertex = RD::Vertex;

    VertexBuffer<GeneralVertex, RD::TERRA_VERT_MAX_ALL> terrain;
    VertexBuffer<GeneralVertex, RD::MODEL_VERT_MAX_ALL> models;
    
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

    void Update(gpu::RenderData& renderData, const res::CpuResources& cpuRes)
    {
        terrain.Reset();
        //TODO: all quadrants
        const auto& verts    = cpuRes.terrain.quadrants[0][0].verts;
        const auto vertCount = cpuRes.terrain.quadrants[0][0].VERT_COUNT_TOTAL;
        terrain.Append(verts, vertCount);

        renderData.debugInfo.vboData_general_vertCount = vertCount;
    }

    void Create(VkCommandPool cmdPool, const res::Resources_Models& resModels)
    {
        terrain.Create();

        models.Create();
        models.Append(resModels.allVertices);
        models.Bake(cmdPool);
    }

    void Destroy()
    {
        terrain.Destroy();
        models.Destroy();
    }
    ~General_Vertices()
    {
        Destroy();
    }

};

}//ns