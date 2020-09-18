//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Objects/BufferExt.hpp"
#include "GPU/VertexData.hpp"

namespace rpg::gpu::vuk {

struct Post_Vertices
{
    using VERTEX_TYPE = PostVertex;
    VertexBuffer<VERTEX_TYPE, VBO_POST_MAX> vbo;

    VkDeviceSize offsets [1] = {};
    const VkVertexInputBindingDescription bindings [1]
    {
        {
            .binding    = 0,
            .stride     = sizeof(VERTEX_TYPE),
            .inputRate  = VK_VERTEX_INPUT_RATE_VERTEX
        }
    };

    const VkVertexInputAttributeDescription attributes [2]
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
        }
    };

    void Update(const com::Array<PostVertex, VBO_POST_MAX>& vboData)
    {
        vbo.Reset(3); //keep the fullscreen triangle
    }

    void Create()
    {
        vbo.Create();

        constexpr VERTEX_TYPE FULLSCREEN_TRIANGLE [3] {
            { {-1,-1 }, {0, 0} },
            { { 3,-1 }, {2, 0} },
            { {-1, 3 }, {0, 2} },
        };
        vbo.Append(FULLSCREEN_TRIANGLE);
    }

    void Destroy()
    {
        vbo.Destroy();
    }

    ~Post_Vertices()
    {
        Destroy();
    }

};

}//ns