//https://github.com/awwdev

#pragma once
#include "Vulkan/Objects/VertexBuffer.hpp"
#include "Common/Structs.hpp"
#include "Common/PrimitiveMeshes.hpp"
#include "Resources/MeshTypes.hpp"
#include "Resources/HostResources.hpp"
#include "Debug/Assert.hpp"

namespace rpg::vk
{
    inline void Default_CreateVertexBuffer(
        VertexBuffer<use::Common_Vertex, gpu::DEFAULT_VERTEX_MAX_COUNT>& vbo, 
        VkCommandPool cmdPool,
        res::HostResources& res)
    {  
        vbo.Create();

        //order matters
        for(u32 i = 0; i < (u32)res::MeshType::ENUM_END; ++i){
            dbg::Assert(res.models.vertexLookup.Contains(i), "mesh type not existent");
            const auto& mesh = res.models.vertexLookup.Get(i);
            vbo.StoreGroup(mesh.begin, mesh.count);
        }
        vbo.Bake(cmdPool);

        vbo.bindings.Append(VkVertexInputBindingDescription{
            .binding    = 0,
            .stride     = sizeof(use::Common_Vertex),
            .inputRate  = VK_VERTEX_INPUT_RATE_VERTEX
        });

        vbo.attributes.Append(VkVertexInputAttributeDescription{
            .location   = 0,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32_SFLOAT,
            .offset     = offsetof(use::Common_Vertex, pos),
        });
        vbo.attributes.Append(VkVertexInputAttributeDescription{
            .location   = 1,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32_SFLOAT,
            .offset     = offsetof(use::Common_Vertex, nor),
        });
        vbo.attributes.Append(VkVertexInputAttributeDescription{
            .location   = 2,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset     = offsetof(use::Common_Vertex, col),
        });
        vbo.attributes.Append(VkVertexInputAttributeDescription{
            .location   = 3,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32_SFLOAT,
            .offset     = offsetof(use::Common_Vertex, tex),
        });
    }

}//ns