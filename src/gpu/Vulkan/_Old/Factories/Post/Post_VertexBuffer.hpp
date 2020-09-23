//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/_Old/Objects/VertexBuffer.hpp"
#include "com/Structs.hpp"
#include "res/Models/Primitives.hpp"
#include "res/_Old/CpuResources.hpp"
#include "res/_Old/CpuResources.hpp"
#include "dbg/Assert.hpp"

namespace rpg::gpu::vuk {

using Post_VertexBuffer = VertexBufferOld<com::Post_Vertex, gpu::UI_VBO_BLUR_COUNT>;

inline void Post_CreateVertexBuffer(Post_VertexBuffer& vbo, VkCommandPool cmdPool)
{  
    vbo.Create();

    vbo.bindings.Append(VkVertexInputBindingDescription{
        .binding    = 0,
        .stride     = sizeof(com::Post_Vertex),
        .inputRate  = VK_VERTEX_INPUT_RATE_VERTEX
    });

    vbo.attributes.Append(VkVertexInputAttributeDescription{
        .location   = 0,
        .binding    = 0, 
        .format     = VK_FORMAT_R32G32_SFLOAT,
        .offset     = offsetof(com::Post_Vertex, pos),
    });
    vbo.attributes.Append(VkVertexInputAttributeDescription{
        .location   = 1,
        .binding    = 0, 
        .format     = VK_FORMAT_R32G32_SFLOAT,
        .offset     = offsetof(com::Post_Vertex, tex),
    });
    vbo.attributes.Append(VkVertexInputAttributeDescription{
        .location   = 2,
        .binding    = 0, 
        .format     = VK_FORMAT_R32_SINT,
        .offset     = offsetof(com::Post_Vertex, blur),
    });

}

}//ns