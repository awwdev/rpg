//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Abstraction/Meta/Context.hpp"
#include "gpu/Vulkan/Abstraction/Objects/BufferExt.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "com/box/Array.hpp"

namespace rpg::gpu::vuk {

struct Post_Vertices
{
    using VERTEX_TYPE = RenderData_Post::Vertex;
    VertexBuffer<VERTEX_TYPE, RenderData_Post::VBO_BLUR_MAX> gpuVbo;

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
            .format     = VK_FORMAT_R32G32_SFLOAT,
            .offset     = offsetof(VERTEX_TYPE, pos),
        },
        {
            .location   = 1,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32_SFLOAT,
            .offset     = offsetof(VERTEX_TYPE, tex),
        },
        {
            .location   = 2,
            .binding    = 0, 
            .format     = VK_FORMAT_R32_SFLOAT,
            .offset     = offsetof(VERTEX_TYPE, blur),
        }
    };

    void Update(const com::Array<RenderData_Post::Vertex, RenderData_Post::VBO_BLUR_MAX>& cpuVbo)
    {
        gpuVbo.Reset(3); //keep the fullscreen triangle
        if (!cpuVbo.Empty())
             gpuVbo.Append(cpuVbo.Data(), cpuVbo.Count());
    }

    void Create()
    {
        gpuVbo.Create();

        constexpr VERTEX_TYPE FULLSCREEN_TRIANGLE [3] {
            { {-1,-1 }, {0, 0}, 0 },
            { {-1, 3 }, {0, 2}, 0 },
            { { 3,-1 }, {2, 0}, 0 },
        };
        gpuVbo.Append(FULLSCREEN_TRIANGLE);
    }

    void Destroy()
    {
        gpuVbo.Destroy();
    }

    ~Post_Vertices()
    {
        Destroy();
    }

};

}//ns