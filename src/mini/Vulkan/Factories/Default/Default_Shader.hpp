//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Objects/Shader.hpp"
#include "mini/Vulkan/Objects/UniformBuffer.hpp"
#include "mini/Vulkan/Objects/ImageArray.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/Utils/Structs.hpp"

namespace mini::vk
{
    inline void Default_CreateShaderVertexColor(Shader& shader, RenderPassDepth& rp)
    {  
        shader.CreateShaderModule("res/Shaders/spv/defaultVertexColor.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
        shader.CreateShaderModule("res/Shaders/spv/defaultVertexColor.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

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
                .imageView      = rp.depthImage.view, 
                .imageLayout    = rp.depthImage.layout
            };
        }
    }

    inline void Default_CreateShaderTexture(Shader& shader, RenderPassDepth& rp, ImageArray& imageArray)
    {  
        shader.CreateShaderModule("res/Shaders/spv/defaultTexture.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
        shader.CreateShaderModule("res/Shaders/spv/defaultTexture.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

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
            .compareOp               = VK_COMPARE_OP_ALWAYS, 
            .minLod                  = 0,
            .maxLod                  = 0, 
            .borderColor             = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE, 
            .unnormalizedCoordinates = VK_FALSE
        };
        VkCheck(vkCreateSampler(g_contextPtr->device, &samplerInfo, nullptr, shader.samplers.Append()));

        //SHADOW MAP
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
                .imageView      = rp.depthImage.view, 
                .imageLayout    = rp.depthImage.layout
            };
        }
        //TEXTURE
        {
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
                .sampler        = shader.samplers[0],
                .imageView      = imageArray.view, 
                .imageLayout    = imageArray.layout
            };
        }

    }

    inline void Default_CreateShaderShadow(Shader& shader)
    {  
        shader.CreateShaderModule("res/Shaders/spv/defaultShadow.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
        shader.CreateShaderModule("res/Shaders/spv/defaultShadow.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
    }

}//ns