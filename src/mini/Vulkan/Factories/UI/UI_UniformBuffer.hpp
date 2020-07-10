//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Objects/UniformBuffer.hpp"

namespace mini::vk
{
    template<class T, u32 N>
    void CreateUniformBuffer_Text(UniformBuffer_Array<T, N>& ub)
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