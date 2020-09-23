//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/Objects/BufferExt.hpp"
#include "gpu/Vulkan/Objects/Descriptors.hpp"
#include "gpu/Vulkan/Objects/PushConstant.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "gpu/RenderData/_Old/RenderStructs.hpp"

namespace rpg::gpu::vuk {

struct Shadow_Uniforms
{
    // re-use ubos from general for model matrices??
    UniformInfo infos [1];
    UniformBuffer<RenderData_Shadow::UBO_ShadowMap, 1> uboSun;
    Descriptors descriptors;
    PushConstant<RenderData_Shadow::Push_Cascades> pushConst;

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

    void Update(const RenderData_Shadow::UBO_ShadowMap& uboShadowMap)
    {
        uboSun.Reset();
        uboSun.Append(uboShadowMap);
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