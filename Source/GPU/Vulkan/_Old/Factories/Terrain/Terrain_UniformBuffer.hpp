//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/_Old/Objects/UniformBuffer.hpp"
#include "gpu/RenderData.hpp"

namespace rpg::gpu::vuk {

inline void Terrain_CreateUniformBuffer(UniformBufferOld<gpu::Terrain_UniformData, 1>& ubo)
{
    ubo.Create();

    ubo.info.type = UniformInfo::Buffer;
    ubo.info.binding = {
        .binding            = 1,
        .descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount    = 1,
        .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr,
    };
    ubo.info.bufferInfo = {
        .buffer = ubo.cpuBuffer.buffer, //create buffer beforehand
        .offset = 0,
        .range  = VK_WHOLE_SIZE
    };
}

}//ns