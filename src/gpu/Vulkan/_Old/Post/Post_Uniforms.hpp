//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Context.hpp"
#include "gpu/Vulkan/Wrappers/Descriptors.hpp"
#include "gpu/Vulkan/Wrappers/Image.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "gui/GUI_Base.hpp"

namespace rpg::gpu::vuk {

struct Post_Uniforms
{
    UniformInfo infos [1];
    Descriptors descriptors;
    VkSampler   sampler;

    void Create(Image& finalImage)
    {
        const VkSamplerCreateInfo samplerInfo 
        {
            .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext                   = nullptr,
            .flags                   = 0,
            .magFilter               = VK_FILTER_NEAREST,
            .minFilter               = VK_FILTER_NEAREST, 
            .mipmapMode              = VK_SAMPLER_MIPMAP_MODE_NEAREST,
            .addressModeU            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, 
            .addressModeV            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, 
            .addressModeW            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .mipLodBias              = 0, 
            .anisotropyEnable        = VK_FALSE, 
            .maxAnisotropy           = 0, 
            .compareEnable           = VK_FALSE,
            .compareOp               = VK_COMPARE_OP_ALWAYS, 
            .minLod                  = 0,
            .maxLod                  = 0, 
            .borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK, 
            .unnormalizedCoordinates = VK_FALSE
        };
        VkCheck(vkCreateSampler(g_contextPtr->device, &samplerInfo, nullptr, &sampler));

        infos[0] = {
            .type = UniformInfo::Image,
            .binding {
                .binding            = 0,
                .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .descriptorCount    = 1,
                .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
                .pImmutableSamplers = nullptr,
            },
            .imageInfo {
                .sampler        = sampler,
                .imageView      = finalImage.view,
                .imageLayout    = finalImage.currentLayout
            }
        };

        //? DESCRIPTORS
        descriptors.Create(infos);
    }

    void Destroy()
    {
        vkDestroySampler(g_contextPtr->device, sampler, nullptr);
        descriptors.Destroy();
        FOR_C_ARRAY(infos, i)
            infos[i] = {};
    }

    ~Post_Uniforms()
    {
        Destroy();
    }

};

}//ns