//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Objects/VertexBuffer.hpp"

namespace mini::vk
{
    inline void CreateVertexBuffer_UI(Context& context, VertexBuffer& vb)
    {
        vb.CreateDynamic(context);

        vb.bindings.Append(VkVertexInputBindingDescription{
            .binding    = 0,
            .stride     = sizeof(utils::Vertex),
            .inputRate  = VK_VERTEX_INPUT_RATE_VERTEX,
        });

        vb.attributes.Append(VkVertexInputAttributeDescription{
            //? positions
            .location   = 0,
            .binding    = 0,
            .format     = VK_FORMAT_R32G32B32_SFLOAT,
            .offset     = offsetof(utils::Vertex, pos)
        });
        vb.attributes.Append(VkVertexInputAttributeDescription{
            //? color
            .location   = 1,
            .binding    = 0,
            .format     = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset     = offsetof(utils::Vertex, col)
        });
        vb.attributes.Append(VkVertexInputAttributeDescription{
            //? texture coords
            .location   = 2,
            .binding    = 0,
            .format     = VK_FORMAT_R32G32_SFLOAT,
            .offset     = offsetof(utils::Vertex, tex)
        });
    }

}//ns