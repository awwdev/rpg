//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Objects/VertexBuffer.hpp"
#include "mini/Utils/Structs.hpp"

namespace mini::vk
{
    template<class T, u32 N>
    void CreateVertexBuffer_Default(VertexBufferStatic<T, N>& vbo, VkCommandPool cmdPool)
    {  
        vbo.Create();
        utils::Vertex tris [] = {
            //!important note: W must be 1
            { {  0.0, -0.5, 0.1, 1 }, {}, { 1, 0, 0, 1 }, {} },
            { {  0.5,  0.5, 0.1, 1 }, {}, { 0, 1, 0, 1 }, {} },
            { { -0.5,  0.5, 0.1, 1 }, {}, { 0, 0, 1, 1 }, {} },
        };
        vbo.AppendGroup(tris, 3);
        vbo.Transfer(cmdPool);

        vbo.bindings.Append(VkVertexInputBindingDescription{
            .binding    = 0,
            .stride     = sizeof(T),
            .inputRate  = VK_VERTEX_INPUT_RATE_VERTEX
        });

        vbo.attributes.Append(VkVertexInputAttributeDescription{
            .location   = 0,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset     = offsetof(T, pos),
        });
        vbo.attributes.Append(VkVertexInputAttributeDescription{
            .location   = 1,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset     = offsetof(T, nor),
        });
        vbo.attributes.Append(VkVertexInputAttributeDescription{
            .location   = 2,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset     = offsetof(T, col),
        });
        vbo.attributes.Append(VkVertexInputAttributeDescription{
            .location   = 3,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset     = offsetof(T, tex),
        });
    }

}//ns