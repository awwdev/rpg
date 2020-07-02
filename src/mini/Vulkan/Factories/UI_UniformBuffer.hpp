//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Objects/UniformBuffer.hpp"

namespace mini::vk
{
    template<class T, u32 N>
    inline void CreateUniformBuffer_Text(Context& context, UniformBuffer_Array<T, N>& ub)
    {
        ub.Create(context);

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


/*
inline void CreateUniformBuffer_UI(Context& context, UniformBuffer& ub)
{
    ub.Create(context);

    ub.uniformInfo.layout = {
        .binding            = 1,
        .descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
        .descriptorCount    = 1,
        .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr,
    };

    ub.uniformInfo.type = UniformInfo::Buffer;
    ub.uniformInfo.bufferInfo = {
        .buffer = ub.buffer.buffer, //!important, buffer must be created beforehand so we have valid ptr
        .offset = 0,
        .range  = VK_WHOLE_SIZE
    };

}
*/