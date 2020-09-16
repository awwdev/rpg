//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Objects/Shader.hpp"
#include "GPU/Vulkan/Objects/UniformBuffer.hpp"
#include "GPU/Vulkan/Objects/ImageArray.hpp"
#include "Resources/CpuResources.hpp"
#include "Common/Structs.hpp"

namespace rpg::gpu::vuk {

inline void Terrain_CreateShader(Shader& shader, RenderPassDepth& rp)
{  
    shader.CreateShaderModule("AssetsApp/Shaders/spv/terrain.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("AssetsApp/Shaders/spv/terrain.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

    const VkSamplerCreateInfo samplerInfo {
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
        //shadowMapSampler
        .compareEnable           = VK_TRUE,//VK_FALSE
        .compareOp               = VK_COMPARE_OP_LESS,
        .minLod                  = 0,
        .maxLod                  = 0, 
        .borderColor             = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,//reversed
        .unnormalizedCoordinates = VK_FALSE
    };
    VkCheck(vkCreateSampler(g_contextPtr->device, &samplerInfo, nullptr, shader.samplers.Append()));

    {
        shader.infos.Append();
        auto& info = shader.infos.Last();

        info.type = UniformInfo::Image;
        info.binding =
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
            .imageView      = rp.depthImageArray.view, 
            .imageLayout    = rp.depthImageArray.layout
        };
    }
}

inline void Terrain_CreateShaderWire(Shader& shader)
{  
    shader.CreateShaderModule("AssetsApp/Shaders/spv/terrainWire.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("AssetsApp/Shaders/spv/terrainWire.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
}

inline void Terrain_CreateShaderShadow(Shader& shader)
{  
    shader.CreateShaderModule("AssetsApp/Shaders/spv/terrainShadow.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader.CreateShaderModule("AssetsApp/Shaders/spv/terrainShadow.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
}

}//ns