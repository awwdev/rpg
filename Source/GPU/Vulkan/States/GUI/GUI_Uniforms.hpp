//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Objects/UniformBuffer.hpp"
#include "GPU/Vulkan/Objects/Descriptors.hpp"
#include "GPU/Vulkan/Objects/ImageArray.hpp"
#include "GPU/RenderData.hpp"
#include "Resources/CpuResources.hpp"

namespace rpg::gpu::vuk {

struct GUI_Uniforms
{
    UniformInfo infos [2];
    UniformBuffer_Groups<gpu::UI_UniformData, gpu::UI_UBO_MAX_COUNT> ubo;
    VkSampler sampler;
    Descriptors descriptors;

    ImageArray fontImages;

    void Create(res::HostResources& hostRes, VkCommandPool cmdPool)
    {
        fontImages.Create(hostRes.textures.monospaceFont, cmdPool);

        //? SAMPLER
        const VkSamplerCreateInfo samplerInfo 
        {
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
        VkCheck(vkCreateSampler(g_contextPtr->device, &samplerInfo, nullptr, &sampler));

        infos[0].type = UniformInfo::Type::Image;
        infos[0].binding =
        {
            .binding            = 0,
            .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount    = 1,
            .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
            .pImmutableSamplers = nullptr,
        };
        infos[0].imageInfo = 
        {
            .sampler        = sampler,
            .imageView      = fontImages.view,
            .imageLayout    = fontImages.layout
        };

        //? UBO
        ubo.Create();

        infos[1].type = UniformInfo::Buffer;
        infos[1].binding = {
            .binding            = 1,
            .descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount    = 1,
            .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT,
            .pImmutableSamplers = nullptr,
        };
        infos[1].bufferInfo = {
            .buffer = ubo.buffer.buffer,
            .offset = 0,
            .range  = VK_WHOLE_SIZE
        };

        //? DESCRIPTORS
        descriptors.Create(infos);
    }

    void Clear()
    {
        ubo.Clear();
        vkDestroySampler(g_contextPtr->device, sampler, nullptr);
        descriptors.Clear();
        fontImages.Clear();
    }

    ~GUI_Uniforms()
    {
        Clear();
    }

};

}//ns