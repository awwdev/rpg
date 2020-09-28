//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/Objects/BufferExt.hpp"
#include "gpu/Vulkan/Objects/Descriptors.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "gpu/RenderData/_Old/RenderStructs.hpp"

namespace rpg::gpu::vuk {

struct General_Uniforms
{
    enum Bindings : uint32_t {
        BindingMeta        = 0,
        BindingModelInstances       = 1,
        BindingSun         = 2,
        BindingShadowMap   = 3,
        ENUM_END
    };

    UniformInfo infos [Bindings::ENUM_END];
    Descriptors descriptors;

    using RD = RenderData_General;

    UniformBuffer<RD::Meta, 1> uboMeta;
    StorageBuffer<RD::ModelInstance, RD::MODEL_INST_MAX> sboModelInstances;
    VkSampler shadowMapSampler;

    void Create(Buffer& uboSun, Image& shadowMaps)
    {
        //? UBO META
        uboMeta.Create();
        infos[BindingMeta] = {
            .type = UniformInfo::Buffer,
            .binding {
                .binding            = BindingMeta,
                .descriptorType     = uboMeta.DESCRIPTOR_TYPE,
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

        //? UBO MODELS
        sboModelInstances.Create();
        infos[BindingModelInstances] = {
            .type = UniformInfo::Buffer,
            .binding {
                .binding            = BindingModelInstances,
                .descriptorType     = sboModelInstances.DESCRIPTOR_TYPE,
                .descriptorCount    = 1,
                .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT,
                .pImmutableSamplers = nullptr,
            },
            .bufferInfo = {
                .buffer = sboModelInstances.activeBuffer->buffer,
                .offset = 0,
                .range  = VK_WHOLE_SIZE,
            }
        };

        //? SUN
        infos[BindingSun] = {
            .type = UniformInfo::Buffer,
            .binding {
                .binding            = BindingSun,
                .descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount    = 1,
                .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT,
                .pImmutableSamplers = nullptr,
            },
            .bufferInfo = {
                .buffer = uboSun.buffer,
                .offset = 0,
                .range  = VK_WHOLE_SIZE,
            }
        };

         //? SHADOW MAP
        const VkSamplerCreateInfo samplerInfo 
        {
            .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext                   = nullptr,
            .flags                   = 0,
            .magFilter               = VK_FILTER_LINEAR,
            .minFilter               = VK_FILTER_LINEAR, 
            .mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR,
            .addressModeU            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, 
            .addressModeV            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, 
            .addressModeW            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
            .mipLodBias              = 0, 
            .anisotropyEnable        = VK_FALSE, 
            .maxAnisotropy           = 0, 
            .compareEnable           = VK_TRUE,
            .compareOp               = VK_COMPARE_OP_LESS, 
            .minLod                  = 0,
            .maxLod                  = 0, 
            .borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK, 
            .unnormalizedCoordinates = VK_FALSE
        };
        VkCheck(vkCreateSampler(g_contextPtr->device, &samplerInfo, nullptr, &shadowMapSampler));

        infos[BindingShadowMap] = {
            .type = UniformInfo::Image,
            .binding {
                .binding            = BindingShadowMap,
                .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .descriptorCount    = 1,
                .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
                .pImmutableSamplers = nullptr,
            },
            .imageInfo {
                .sampler        = shadowMapSampler,
                .imageView      = shadowMaps.view,
                .imageLayout    = shadowMaps.layout
            }
        };

        //? DESCRIPTORS
        descriptors.Create(infos);
    }

    void Update(RenderData_General& rdGeneral)
    {
        uboMeta.Reset();
        uboMeta.Append(rdGeneral.meta);

        sboModelInstances.Reset();
        if (!rdGeneral.modelInstances.Empty())
            sboModelInstances.Append(rdGeneral.modelInstances);
    }

    void Destroy()
    {
        uboMeta.Destroy();
        sboModelInstances.Destroy();
        descriptors.Destroy();
        vkDestroySampler(g_contextPtr->device, shadowMapSampler, nullptr);
        FOR_CARRAY(infos, i)
            infos[i] = {};
    }
    ~General_Uniforms()
    {
        Destroy();
    }

};

}//ns