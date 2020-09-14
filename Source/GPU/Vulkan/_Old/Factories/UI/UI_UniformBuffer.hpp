//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Objects/UniformBuffer.hpp"
#include "GPU/RenderData.hpp"

namespace rpg::gpu::vuk {

inline void UI_CreateUniformBuffer(
    UniformBuffer_Groups<gpu::UI_UniformData, gpu::UI_UBO_MAX_COUNT>& ub)
{
    ub.Create();

    ub.info.type = UniformInfo::Buffer;
    ub.info.layout = {
        .binding            = 1,
        .descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount    = 1,
        .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT,
        .pImmutableSamplers = nullptr,
    };
    ub.info.bufferInfo = {
        .buffer = ub.buffer.buffer, //create buffer beforehand
        .offset = 0,
        .range  = VK_WHOLE_SIZE
    };
}

}//ns