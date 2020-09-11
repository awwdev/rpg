//https://github.com/awwdev

#pragma once
#include "Vulkan/Context.hpp"
#include "Vulkan/Objects/UniformBuffer.hpp"
#include "Rendering/RenderData.hpp"

namespace rpg::vk {

inline void Terrain_CreateUniformBuffer(UniformBuffer<gpu::Terrain_UniformData, 1>& ubo)
{
    ubo.Create();

    ubo.info.type = UniformInfo::Buffer;
    ubo.info.layout = {
        .binding            = 1,
        .descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount    = 1,
        .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr,
    };
    ubo.info.bufferInfo = {
        .buffer = ubo.buffer.buffer, //create buffer beforehand
        .offset = 0,
        .range  = VK_WHOLE_SIZE
    };
}

}//ns