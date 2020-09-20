//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/_Old/Objects/VertexBuffer.hpp"
#include "Common/Structs.hpp"
#include "Common/PrimitiveMeshes.hpp"
#include "Resources/MeshTypes.hpp"
#include "Resources/CpuResources.hpp"
#include "Debug/Assert.hpp"

namespace rpg::gpu::vuk {

inline void Terrain_CreateVertexBuffer(
    VertexBufferOld<com::Common_Vertex, gpu::TERRAIN_VERTEX_MAX_COUNT>& vbo, 
    VkCommandPool,
    res::CpuResources& res)
{  
    vbo.Create();

    for(idx_t z = 0; z < res.terrain.QUADRANT_COUNT; ++z) {
    for(idx_t x = 0; x < res.terrain.QUADRANT_COUNT; ++x) {
        vbo.StoreGroup(res.terrain.quadrants[z][x].verts);
    }}

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