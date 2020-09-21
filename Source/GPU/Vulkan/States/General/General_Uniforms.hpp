//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Objects/BufferExt.hpp"
#include "GPU/Vulkan/Objects/Descriptors.hpp"
#include "GPU/RenderData.hpp"
#include "GPU/RenderStructs.hpp"

namespace rpg::gpu::vuk {

struct General_Uniforms
{
    UniformInfo infos [3];
    Descriptors descriptors;

    UniformBuffer<UboData_General_Meta, 1> uboMeta;
    VkSampler shadowMapSampler;

    void Create(Buffer& uboSun, Image& shadowMaps)
    {
        //? UBO META
        uboMeta.Create();
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

        //? SHADOW MAP
        infos[1] = {
            .type = UniformInfo::Buffer,
            .binding {
                .binding            = 1,
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

        infos[2] = {
            .type = UniformInfo::Image,
            .binding {
                .binding            = 2,
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

    void Update(UboData_General_Meta& uboData_general_meta)
    {
        uboMeta.Reset();
        uboMeta.Append(uboData_general_meta);
    }

    void Destroy()
    {
        uboMeta.Destroy();
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