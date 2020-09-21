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

struct Shadow_Vertices
{
    using VERTEX_TYPE = GeneralVertex;
    //using form general vbo

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
    }

    void Create()
    {
    }

    void Destroy()
    {
    }
    ~Shadow_Vertices()
    {
        Destroy();
    }

};

}//ns