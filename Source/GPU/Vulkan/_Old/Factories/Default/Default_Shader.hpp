//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Objects/Shader.hpp"
#include "GPU/Vulkan/Objects/UniformBuffer.hpp"
#include "GPU/Vulkan/Objects/ImageArray.hpp"
#include "Resources/CpuResources.hpp"
#include "Common/Structs.hpp"

namespace rpg::gpu::vuk {
    
inline void Default_CreateShaderVertexColor(Shader& shader, RenderPassDepth& rp)
{  
    shader.CreateShaderModule("AssetsApp/Shaders/spv/defaultVertexColor.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("AssetsApp/Shaders/spv/defaultVertexColor.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

    const VkSamplerCreateInfo samplerInfo {
        .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .pNext                   = nullptr,
        .flags                   = 0,
        .magFilter               = VK_FILTER_NEAREST,
        .minFilter               = VK_FILTER_NEAREST, 
        .mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR,
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
        .borderColor             = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE, 
        .unnormalizedCoordinates = VK_FALSE
    };
    VkCheck(vkCreateSampler(g_contextPtr->device, &samplerInfo, nullptr, shader.samplers.Append()));

    {
        shader.infos.Append();
        auto& info = shader.infos.Last();

        info.type = UniformInfo::Image;
        info.layout =
        {
            .binding            = 1,
            .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount    = 1,
            .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
            .pImmutableSamplers = nullptr,
        };
        info.imageInfo = 
        {
            .sampler        = shader.samplers[0],
            .imageView      = rp.depthImageArray.view, 
            .imageLayout    = rp.depthImageArray.layout
        };
    }
}

inline void Default_CreateShaderTexture(Shader& shader, RenderPassDepth& rp, ImageArray& imageArray)
{  
    shader.CreateShaderModule("AssetsApp/Shaders/spv/defaultTexture.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("AssetsApp/Shaders/spv/defaultTexture.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

    //SHADOW MAP
    {
        const VkSamplerCreateInfo samplerInfo {
            .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext                   = nullptr,
            .flags                   = 0,
            .magFilter               = VK_FILTER_LINEAR,
            .minFilter               = VK_FILTER_LINEAR,
            .mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR,
            .addressModeU            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, 
            .addressModeV            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, 
            .addressModeW            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .mipLodBias              = 0, 
            .anisotropyEnable        = VK_FALSE, 
            .maxAnisotropy           = 1, 
            .compareEnable           = VK_TRUE,
            .compareOp               = VK_COMPARE_OP_LESS, 
            .minLod                  = 0,
            .maxLod                  = 0, 
            .borderColor             = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE, 
            .unnormalizedCoordinates = VK_FALSE
        };
        VkCheck(vkCreateSampler(g_contextPtr->device, &samplerInfo, nullptr, shader.samplers.Append()));

        shader.infos.Append();
        auto& info = shader.infos.Last();

        info.type = UniformInfo::Image;
        info.layout =
        {
            .binding            = 1,
            .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount    = 1,
            .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
            .pImmutableSamplers = nullptr,
        };
        info.imageInfo = 
        {
            .sampler        = shader.samplers.Last(),
            .imageView      = rp.depthImageArray.view, 
            .imageLayout    = rp.depthImageArray.layout
        };
    }
    //TEXTURE
    {
        const VkSamplerCreateInfo samplerInfo {
            .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext                   = nullptr,
            .flags                   = 0,
            .magFilter               = VK_FILTER_LINEAR,
            .minFilter               = VK_FILTER_LINEAR, //VK_FILTER_NEAREST
            .mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR, //VK_SAMPLER_MIPMAP_MODE_NEAREST
            .addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT, 
            .addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT, 
            .addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT,//VK_SAMPLER_ADDRESS_MODE_REPEAT
            .mipLodBias              = 0, 
            .anisotropyEnable        = VK_FALSE, 
            .maxAnisotropy           = 1, 
            .compareEnable           = VK_FALSE,
            .compareOp               = VK_COMPARE_OP_LESS, 
            .minLod                  = 0,
            .maxLod                  = 0, 
            .borderColor             = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE, 
            .unnormalizedCoordinates = VK_FALSE
        };
        VkCheck(vkCreateSampler(g_contextPtr->device, &samplerInfo, nullptr, shader.samplers.Append()));

        shader.infos.Append();
        auto& info = shader.infos.Last();

        info.type = UniformInfo::Image;
        info.layout =
        {
            .binding            = 2,
            .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount    = 1,
            .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
            .pImmutableSamplers = nullptr,
        };
        info.imageInfo = 
        {
            .sampler        = shader.samplers.Last(),
            .imageView      = imageArray.view, 
            .imageLayout    = imageArray.layout
        };
    }

}

inline void Default_CreateShaderShadow(Shader& shader)
{  
    shader.CreateShaderModule("AssetsApp/Shaders/spv/defaultShadow.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("AssetsApp/Shaders/spv/defaultShadow.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
}

}//ns