//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Objects/VertexBuffer.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Utils/PrimitiveMeshes.hpp"
#include "mini/Resources/Mesh.hpp"
#include "mini/Resources/HostResources.hpp"

namespace mini::vk
{
    inline void CreateVertexBuffer_Default(
        VertexBuffer_Static<utils::VertexDefault, rendering::DEFAULT_VERTEX_MAX_COUNT>& vbo, 
        VkCommandPool cmdPool,
        res::HostResources& res)
    {  
        vbo.Create();

        //order matters
        for(auto i = 0; i < res::MeshType::ENUM_END; ++i){
            const auto& mesh = res.models.meshvertexLookup.Get(i);
            vbo.AppendGroup(mesh.begin, mesh.count);
        }
        vbo.Transfer(cmdPool);

        vbo.bindings.Append(VkVertexInputBindingDescription{
            .binding    = 0,
            .stride     = sizeof(utils::VertexDefault),
            .inputRate  = VK_VERTEX_INPUT_RATE_VERTEX
        });

        vbo.attributes.Append(VkVertexInputAttributeDescription{
            .location   = 0,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset     = offsetof(utils::VertexDefault, pos),
        });
        vbo.attributes.Append(VkVertexInputAttributeDescription{
            .location   = 1,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset     = offsetof(utils::VertexDefault, nor),
        });
        vbo.attributes.Append(VkVertexInputAttributeDescription{
            .location   = 2,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset     = offsetof(utils::VertexDefault, col),
        });
        vbo.attributes.Append(VkVertexInputAttributeDescription{
            .location   = 3,
            .binding    = 0, 
            .format     = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset     = offsetof(utils::VertexDefault, tex),
        });
    }

}//ns