//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Objects/BufferExt.hpp"
#include "GPU/RenderData.hpp"
#include "GPU/VertexData.hpp"
#include "Common/Container/Array.hpp"

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

    void Update(const com::Array<PostVertex, VBO_POST_MAX>& cpuVbo)
    {
        //gpuVbo.Reset(3); //keep the fullscreen triangle
        //gpuVbo.Append(cpuVbo.Data(), cpuVbo.count);
    }

    void Create()
    {
        gpuVbo.Create();
        constexpr VERTEX_TYPE TRIANGLE [3] {
            { .pos {  0.0,-0.5, 0 }, .nor {}, .col { 1, 1, 1, 1 }, .tex {} },
            { .pos {  0.5, 0.5, 0 }, .nor {}, .col { 1, 1, 1, 1 }, .tex {} },
            { .pos { -0.5, 0.5, 0 }, .nor {}, .col { 1, 1, 1, 1 }, .tex {} },
        };
        gpuVbo.Append(TRIANGLE);
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