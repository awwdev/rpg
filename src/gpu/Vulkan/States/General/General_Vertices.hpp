//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/Objects/BufferExt.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "gpu/RenderData/_Old/RenderStructs.hpp"
#include "com/box/Array.hpp"

#include "res/_Old/CpuResources.hpp"
#include "gpu/RenderData/RenderData.hpp"

namespace rpg::gpu::vuk {

struct General_Vertices
{
    using VERTEX_TYPE = RenderData_General::Vertex;
    VertexBuffer<VERTEX_TYPE, RenderData_General::VBO_TERRAIN_MAX> terrain;
    VertexBuffer<VERTEX_TYPE, RenderData_General::VBO_OBJECTS_MAX> objects;

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
        terrain.Reset();
        
        const auto& verts    = cpuRes.terrain.quadrants[0][0].verts;
        const auto vertCount = cpuRes.terrain.quadrants[0][0].VERT_COUNT_TOTAL;
        terrain.Append(verts, vertCount);

        renderData.debugInfo.vboData_general_vertCount = vertCount;
    }

    void Create(VkCommandPool cmdPool, const res::CpuResources& cpuRes)
    {
        terrain.Create();

        objects.Create();
        objects.Append(cpuRes.models.allModelVertices);
        objects.Bake(cmdPool);
    }

    void Destroy()
    {
        terrain.Destroy();
        objects.Destroy();
    }
    ~General_Vertices()
    {
        Destroy();
    }

};

}//ns