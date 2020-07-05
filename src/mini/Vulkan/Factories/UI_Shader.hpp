//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Objects/Shader.hpp"
#include "mini/Vulkan/Objects/UniformBuffer.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/Utils/Structs.hpp"

namespace mini::vk
{
    inline void CreateShader_Text(Context& context, Shader& shader, ImageArray& imageArray)
    {  
        shader.device = context.device;

        shader.CreateShaderModule("res/Shaders/spv/text.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
        shader.CreateShaderModule("res/Shaders/spv/text.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

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
        VK_CHECK(vkCreateSampler(shader.device, &samplerInfo, nullptr, &shader.samplers.AppendReturn()));

        shader.info.type = UniformInfo::Image;
        shader.info.layout =
        {
            .binding            = 0,
            .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount    = 1,
            .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
            .pImmutableSamplers = nullptr,
        };
        shader.info.imageInfo = 
        {
            .sampler        = shader.samplers[0],
            .imageView      = imageArray.view, 
            .imageLayout    = imageArray.layout
        };

    }

}//ns






/*
inline void CreateShader_UI(Context& context, Shader& shader, Image images[]) //or pass some upper struct UBOS
{  
    shader.CreateShaderModule("res/Shaders/spv/ui.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("res/Shaders/spv/ui.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

    const VkSamplerCreateInfo samplerInfo {
        .sType                  = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .pNext                  = nullptr,
        .flags                  = 0,
        .magFilter              = VK_FILTER_NEAREST,//VK_FILTER_LINEAR,
        .minFilter              = VK_FILTER_NEAREST,//VK_FILTER_LINEAR, 
        .mipmapMode             = VK_SAMPLER_MIPMAP_MODE_NEAREST,//VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .addressModeU           = VK_SAMPLER_ADDRESS_MODE_REPEAT, 
        .addressModeV           = VK_SAMPLER_ADDRESS_MODE_REPEAT, 
        .addressModeW           = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .mipLodBias             = 0, 
        .anisotropyEnable       = VK_FALSE, 
        .maxAnisotropy          = 16.0f, 
        .compareEnable          = VK_FALSE,
        .compareOp              = VK_COMPARE_OP_ALWAYS, 
        .minLod                 = 0,
        .maxLod                 = 0, 
        .borderColor            = VK_BORDER_COLOR_INT_OPAQUE_BLACK, 
        .unnormalizedCoordinates = VK_FALSE
    };
    VK_CHECK(vkCreateSampler(shader.device, &samplerInfo, nullptr, &shader.samplers.AppendReturn()));

    shader.info.type = UniformInfo::Image;
    shader.info.layout =
    {
        .binding            = 0,
        .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount    = 1,
        .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr,
    };

    auto& image = images[hostRes::Font];
    shader.info.imageInfo = 
    {
        .sampler        = shader.samplers[0],
        .imageView      = image.view, 
        .imageLayout    = image.layout
    };

}


inline void CreateShader_UI_wire(Context& context, Shader& shader, Image images[]) //or pass some upper struct UBOS
{  
    shader.CreateShaderModule("res/Shaders/spv/ui_wire.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("res/Shaders/spv/ui_wire.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

    const VkSamplerCreateInfo samplerInfo {
        .sType                  = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .pNext                  = nullptr,
        .flags                  = 0,
        .magFilter              = VK_FILTER_NEAREST,//VK_FILTER_LINEAR,
        .minFilter              = VK_FILTER_NEAREST,//VK_FILTER_LINEAR, 
        .mipmapMode             = VK_SAMPLER_MIPMAP_MODE_NEAREST,//VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .addressModeU           = VK_SAMPLER_ADDRESS_MODE_REPEAT, 
        .addressModeV           = VK_SAMPLER_ADDRESS_MODE_REPEAT, 
        .addressModeW           = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .mipLodBias             = 0, 
        .anisotropyEnable       = VK_FALSE, 
        .maxAnisotropy          = 16.0f, 
        .compareEnable          = VK_FALSE,
        .compareOp              = VK_COMPARE_OP_ALWAYS, 
        .minLod                 = 0,
        .maxLod                 = 0, 
        .borderColor            = VK_BORDER_COLOR_INT_OPAQUE_BLACK, 
        .unnormalizedCoordinates = VK_FALSE
    };
    VK_CHECK(vkCreateSampler(shader.device, &samplerInfo, nullptr, &shader.samplers.AppendReturn()));

    shader.info.type = UniformInfo::Image;
    shader.info.layout =
    {
        .binding            = 0,
        .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount    = 1,
        .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr,
    };

    auto& image = images[hostRes::Font];
    shader.info.imageInfo = 
    {
        .sampler        = shader.samplers[0],
        .imageView      = image.view, 
        .imageLayout    = image.layout
    };

}
*/