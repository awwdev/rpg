//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/_Old/Objects/VertexBuffer.hpp"

#include "res/Models/ModelPrimitives.hpp"
#include "res/_Old/CpuResources.hpp"
#include "res/_Old/CpuResources.hpp"
#include "dbg/Assert.hpp"

namespace rpg::gpu::vuk
{
    inline void Common_CreateVertexBuffer(
        VertexBufferOld<com::Common_Vertex, gpu::DEFAULT_VERTEX_MAX_COUNT>& vbo, 
        VkCommandPool cmdPool,
        res::CpuResources& res)
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
            .stride     = sizeof(com::Common_Vertex),
            .inputRate  = VK_VERTEX_INPUT_RATE_VERTEX
        });

        vbo.attributes.Append(VkVertexInputAttributeDescription{
            .location   = 0,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32_SFLOAT,
            .offset     = offsetof(com::Common_Vertex, pos),
        });
        vbo.attributes.Append(VkVertexInputAttributeDescription{
            .location   = 1,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32_SFLOAT,
            .offset     = offsetof(com::Common_Vertex, nor),
        });
        vbo.attributes.Append(VkVertexInputAttributeDescription{
            .location   = 2,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset     = offsetof(com::Common_Vertex, col),
        });
        vbo.attributes.Append(VkVertexInputAttributeDescription{
            .location   = 3,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32_SFLOAT,
            .offset     = offsetof(com::Common_Vertex, tex),
        });
    }

}//ns