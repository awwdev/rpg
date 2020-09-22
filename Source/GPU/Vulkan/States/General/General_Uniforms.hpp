//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Objects/BufferExt.hpp"
#include "GPU/Vulkan/Objects/Descriptors.hpp"
#include "GPU/RenderData/RenderData.hpp"
#include "GPU/RenderData/_Old/RenderStructs.hpp"

namespace rpg::gpu::vuk {

struct General_Uniforms
{
    enum Bindings : uint32_t {
        BindingMeta        = 0,
        BindindModel       = 1,
        BindingSun         = 2,
        BindingShadowMap   = 3,
        ENUM_END
    };

    UniformInfo infos [Bindings::ENUM_END];
    Descriptors descriptors;

    using RD = RenderData_General;

    UniformBuffer<RD::UBO_Meta, 1> uboMeta;
    StorageBuffer<RD::SBO_Model, RD::UBO_MODEL_MAX> sboModel;
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
        sboModel.Create();
        infos[BindindModel] = {
            .type = UniformInfo::Buffer,
            .binding {
                .binding            = BindindModel,
                .descriptorType     = sboModel.DESCRIPTOR_TYPE,
                .descriptorCount    = 1,
                .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT,
                .pImmutableSamplers = nullptr,
            },
            .bufferInfo = {
                .buffer = sboModel.activeBuffer->buffer,
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
        uboMeta.Append(rdGeneral.uboMeta);

        sboModel.Reset();
        sboModel.Append(rdGeneral.sboModels);
    }

    void Destroy()
    {
        uboMeta.Destroy();
        sboModel.Destroy();
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