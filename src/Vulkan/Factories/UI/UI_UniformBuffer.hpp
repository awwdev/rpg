//https://github.com/awwdev

#pragma once
#include "Vulkan/Context.hpp"
#include "Vulkan/Objects/UniformBuffer.hpp"
#include "Rendering/RenderGraph.hpp"

namespace rpg::vk {

inline void UI_CreateUniformBuffer(
    UniformBuffer_Groups<rendering::UI_UniformData, rendering::UI_UBO_MAX_COUNT>& ub)
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