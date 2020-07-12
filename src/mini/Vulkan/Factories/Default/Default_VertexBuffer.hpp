//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Objects/VertexBuffer.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Utils/PrimitiveMeshes.hpp"
#include "mini/Resources/Mesh.hpp"

namespace mini::vk
{
    //!requirement: resources manager has loaded all meshes
    template<class T, u32 N>
    void CreateVertexBuffer_Default(VertexBuffer_Static<T, N>& vbo, VkCommandPool cmdPool)
    {  
        vbo.Create();
        for(auto i = 0; i < resources::MeshType::ENUM_END; ++i){
            //vbo.AppendGroup(MESH_CUBE);//TODO: MESH MAPPING
        }
        vbo.AppendGroup(MESH_CUBE);//remove, do mapping instead!
        vbo.AppendGroup(MESH_QUAD);
        vbo.AppendGroup(MESH_TRIANGLE);
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