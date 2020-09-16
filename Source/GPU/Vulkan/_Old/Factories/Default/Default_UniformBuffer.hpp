//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Objects/UniformBuffer.hpp"
#include "GPU/UboData.hpp"
#include "Common/Structs.hpp"
#include "GPU/RenderData.hpp"

namespace rpg::gpu::vuk
{
    void Common_CreateUniformBuffer(
        UniformBuffer_Groups<gpu::Common_UniformData, 
        gpu::DEFAULT_UBO_MAX_COUNT>& ubo)
    {  
        ubo.Create();

        ubo.info.type = UniformInfo::Buffer;
        ubo.info.binding = {
            .binding            = 0,
            .descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount    = 1,
            .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT,
            .pImmutableSamplers = nullptr,
        };
        ubo.info.bufferInfo = {
            .buffer = ubo.buffer.buffer, //create buffer beforehand
            .offset = 0,
            .range  = VK_WHOLE_SIZE
        };
    }

}//ns