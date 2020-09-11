//https://github.com/awwdev

#pragma once
#include "Vulkan/Objects/UniformBuffer.hpp"
#include "Rendering/UboData.hpp"
#include "Utils/Structs.hpp"
#include "Rendering/RenderData.hpp"

namespace rpg::vk
{
    void Default_CreateUniformBuffer(
        UniformBuffer_Groups<gpu::Default_UniformData, 
        gpu::DEFAULT_UBO_MAX_COUNT>& ubo)
    {  
        ubo.Create();

        ubo.info.type = UniformInfo::Buffer;
        ubo.info.layout = {
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