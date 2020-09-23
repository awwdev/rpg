//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/_Old/Objects/UniformBuffer.hpp"
#include "gpu/UboData.hpp"
#include "com/Structs.hpp"
#include "gpu/RenderData.hpp"

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