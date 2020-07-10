//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Objects/UniformBuffer.hpp"
#include "mini/Rendering/UboData.hpp"
#include "mini/Utils/Structs.hpp"

namespace mini::vk
{
    template<class T, u32 N>
    void CreateUniformBuffer_Default(UniformBuffer_Groups<T, N>& ubo)
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