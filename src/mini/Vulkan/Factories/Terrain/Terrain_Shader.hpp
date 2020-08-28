//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Objects/Shader.hpp"
#include "mini/Vulkan/Objects/UniformBuffer.hpp"
#include "mini/Vulkan/Objects/ImageArray.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/Utils/Structs.hpp"

namespace mini::vk
{
    inline void Terrain_CreateShader(Shader& shader, RenderPassDepth& rp)
    {  
        shader.CreateShaderModule("res/Shaders/spv/terrain.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
        shader.CreateShaderModule("res/Shaders/spv/terrain.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

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
                .binding            = 0,
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

    inline void Terrain_CreateShaderWire(Shader& shader)
    {  
        shader.CreateShaderModule("res/Shaders/spv/terrainWire.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
        shader.CreateShaderModule("res/Shaders/spv/terrainWire.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
    }

    inline void Terrain_CreateShaderShadow(Shader& shader)
    {  
        shader.CreateShaderModule("res/Shaders/spv/terrainShadow.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
        shader.CreateShaderModule("res/Shaders/spv/terrainShadow.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
    }

}//ns