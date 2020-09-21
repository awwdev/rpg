//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Objects/BufferExt.hpp"
#include "GPU/RenderData.hpp"
#include "GPU/RenderStructs.hpp"
#include "Common/Container/Array.hpp"

#include "Resources/CpuResources.hpp"
#include "GPU/RenderData.hpp"

namespace rpg::gpu::vuk {

struct General_Vertices
{
    using VERTEX_TYPE = GeneralVertex;
    VertexBuffer<VERTEX_TYPE, VBO_GENERAL_MAX> gpuVbo;

    VkDeviceSize offsets [1] = {};
    static constexpr VkVertexInputBindingDescription bindings []
    {
        {
            .binding    = 0,
            .stride     = sizeof(VERTEX_TYPE),
            .inputRate  = VK_VERTEX_INPUT_RATE_VERTEX
        }
    };

    static constexpr VkVertexInputAttributeDescription attributes []
    {
        {
            .location   = 0,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32_SFLOAT,
            .offset     = offsetof(VERTEX_TYPE, pos),
        },
        {
            .location   = 1,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32_SFLOAT,
            .offset     = offsetof(VERTEX_TYPE, nor),
        },
        {
            .location   = 2,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset     = offsetof(VERTEX_TYPE, col),
        },
        {
            .location   = 3,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32_SFLOAT,
            .offset     = offsetof(VERTEX_TYPE, tex),
        }
    };

    void Update(gpu::RenderData& renderData, const res::CpuResources& cpuRes)
    {
        gpuVbo.Reset();
        const auto& verts    = cpuRes.terrain.quadrants[0][0].verts;
        const auto vertCount = cpuRes.terrain.quadrants[0][0].VERT_COUNT_TOTAL;
        gpuVbo.Append(verts, vertCount);

        renderData.debugInfo.vboData_general_vertCount = vertCount;
    }

    void Create()
    {
        gpuVbo.Create();
    }

    void Destroy()
    {
        gpuVbo.Destroy();
    }
    ~General_Vertices()
    {
        Destroy();
    }

};

}//ns