//https://github.com/awwdev

#pragma once
#include "Vulkan/Objects/Shader.hpp"
#include "Vulkan/Objects/UniformBuffer.hpp"
#include "Vulkan/Objects/ImageArray.hpp"
#include "Resources/CpuResources.hpp"
#include "Common/Structs.hpp"

namespace rpg::vuk {
    
inline void UI_CreateShader(Shader& shader, ImageArray& imageArray)
{  
    shader.CreateShaderModule("res/Shaders/spv/ui.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("res/Shaders/spv/ui.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

    const VkSamplerCreateInfo samplerInfo {
        .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .pNext                   = nullptr,
        .flags                   = 0,
        .magFilter               = VK_FILTER_NEAREST,
        .minFilter               = VK_FILTER_NEAREST, 
        .mipmapMode              = VK_SAMPLER_MIPMAP_MODE_NEAREST,
        .addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT, 
        .addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT, 
        .addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
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
    VkCheck(vkCreateSampler(g_contextPtr->device, &samplerInfo, nullptr, shader.samplers.Append()));

    {
        shader.infos.Append();
        auto& info = shader.infos.Last();

        info.type = UniformInfo::Image;
        info.layout =
        {
            .binding            = 0,
            .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount    = 1,
            .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
            .pImmutableSamplers = nullptr,
        };
        info.imageInfo = 
        {
            .sampler        = shader.samplers[0],
            .imageView      = imageArray.view, 
            .imageLayout    = imageArray.layout
        };
    }
}

}//NS