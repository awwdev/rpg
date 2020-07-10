//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Objects/UniformBuffer.hpp"
#include "mini/RenderGraph/UboData.hpp"
#include "mini/Utils/Structs.hpp"

namespace mini::vk
{
    template<class T, u32 N>
    void CreateUniformBuffer_Default(UniformBuffer_Groups<T, N>& ubo)
    {  
        ubo.Create();
        rendergraph::UniformData_Default triangles [] = {
            math::Identity4x4(),
        };
        rendergraph::UniformData_Default quads [] = {
            { 
                .1f, 0.f, 0.f, .5f,
                0.f, .1f, 0.f, 0.f,
                0.f, 0.f, .1f, 0.f,
                0.f, 0.f, 0.f, 1.f,            
            },
            { 
                .1f, 0.f, 0.f, -.5f,
                0.f, .1f, 0.f, 0.f,
                0.f, 0.f, .1f, 0.f,
                0.f, 0.f, 0.f, 1.f,            
            },
        };
        ubo.AppendGroup(triangles);
        ubo.AppendGroup(quads);

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