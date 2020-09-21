//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Objects/BufferExt.hpp"
#include "GPU/Vulkan/Objects/Descriptors.hpp"
#include "GPU/Vulkan/Objects/PushConstant.hpp"
#include "GPU/RenderData.hpp"
#include "GPU/RenderStructs.hpp"

namespace rpg::gpu::vuk {

struct Shadow_Uniforms
{
    // re-use ubos from general for model matrices??
    UniformInfo infos [1];
    UniformBuffer<UboData_Shadow_Sun, 1> uboSun;
    Descriptors descriptors;
    PushConstant<PushData_Shadow> pushConst;

    void Create()
    {
        uboSun.Create();

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
                .buffer = uboSun.activeBuffer->buffer,
                .offset = 0,
                .range  = VK_WHOLE_SIZE,
            }
        };

        //? DESCRIPTORS
        descriptors.Create(infos);

        //? PUSH CONSTS
        pushConst.rangeInfo = {
            .stageFlags  = VK_SHADER_STAGE_VERTEX_BIT,
            .offset      = 0,
            .size        = sizeof(pushConst.data)
        };
    }

    void Update(const UboData_Shadow_Sun& uboData_shadow_sun)
    {
        uboSun.Reset();
        uboSun.Append(uboData_shadow_sun);
    }

    void Destroy()
    {
        uboSun.Destroy();
        descriptors.Destroy();
        FOR_CARRAY(infos, i)
            infos[i] = {};
    }
    ~Shadow_Uniforms()
    {
        Destroy();
    }

};

}//ns