//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Objects/BufferExt.hpp"
#include "GPU/Vulkan/Objects/Descriptors.hpp"
#include "GPU/RenderData.hpp"
#include "GPU/UboData.hpp"

namespace rpg::gpu::vuk {

struct General_Uniforms
{
    UniformInfo infos [1];
    UniformBuffer<UboData_General_Meta, 1> uboMeta;
    Descriptors descriptors;

    void Create()
    {
        uboMeta.Create();
        
        //test
        UboData_General_Meta data {
            .view {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
             -0.5, 0, 0, 1,
            }
        };
        uboMeta.Append(data);


        infos[0] = {
            .type = UniformInfo::Buffer,
            .binding {
                .binding            = 0,
                .descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount    = 1,
                .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT,
                .pImmutableSamplers = nullptr,
            },
            .bufferInfo = {
                .buffer = uboMeta.activeBuffer->buffer,
                .offset = 0,
                .range  = VK_WHOLE_SIZE,
            }
        };

        //? DESCRIPTORS
        descriptors.Create(infos);
    }

    void Destroy()
    {
        uboMeta.Destroy();
        descriptors.Destroy();
        infos[0] = {};
    }
    ~General_Uniforms()
    {
        Destroy();
    }

};

}//ns