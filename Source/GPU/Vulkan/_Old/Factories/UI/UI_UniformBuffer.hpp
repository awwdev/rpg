//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/_Old/Objects/UniformBuffer.hpp"
#include "gpu/RenderData.hpp"

namespace rpg::gpu::vuk {

inline void UI_CreateUniformBuffer(
    UniformBuffer_Groups<gpu::RenderData_GUI::UBO_Text, gpu::RenderData_GUI::UBO_GUI_TEXT_MAX>& ub)
{
    ub.Create();

    ub.info.type = UniformInfo::Buffer;
    ub.info.binding = {
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