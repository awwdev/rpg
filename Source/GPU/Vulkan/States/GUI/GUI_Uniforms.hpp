//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/_Old/Objects/UniformBuffer.hpp"
#include "GPU/Vulkan/Objects/UniformBuffer.hpp"
#include "GPU/Vulkan/_Old/Objects/Descriptors.hpp"
#include "GPU/Vulkan/_Old/Objects/ImageArray.hpp"
#include "GPU/RenderData.hpp"
#include "Resources/CpuResources.hpp"
#include "GUI/GUI_Base.hpp"

namespace rpg::gpu::vuk {

struct GUI_Uniforms
{
    UniformInfo infos [3];
    UniformBuffer2<gpu::UI_UniformData, gpu::UI_UBO_MAX_COUNT> uboText;
    UniformBuffer2<GUI_UBO_Colors, 1> uboColors;

    VkSampler sampler;
    ImageArray fontImages;

    Descriptors descriptors;

    void Create(res::HostResources& hostRes, VkCommandPool cmdPool)
    {
        fontImages.Create(hostRes.textures.monospaceFont, cmdPool);

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
                .imageView      = fontImages.view,
                .imageLayout    = fontImages.layout
            }
        };

        uboText.Create();
        infos[1] = {
            .type = UniformInfo::Buffer,
            .binding {
                .binding            = 1,
                .descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount    = 1,
                .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT,
                .pImmutableSamplers = nullptr,
            },
            .bufferInfo {
                .buffer = uboText.activeBuffer->buffer,
                .offset = 0,
                .range  = VK_WHOLE_SIZE
            }
        };

        uboColors.Create();
        GUI_UBO_Colors colors;
        std::memcpy(colors.colors, gui::ColorValues.data, gui::ColorValues.ENUM_END * sizeof(com::Vec4f));
        uboColors.Append(colors);
        uboColors.Bake(cmdPool);
        
        infos[2] = {
            .type = UniformInfo::Buffer,
            .binding {
                .binding            = 2,
                .descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount    = 1,
                .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT,
                .pImmutableSamplers = nullptr,
            },
            .bufferInfo {
                .buffer = uboColors.activeBuffer->buffer,
                .offset = 0,
                .range  = VK_WHOLE_SIZE
            }
        };

        //? DESCRIPTORS
        descriptors.Create(infos);
    }

    void Clear()
    {
        uboText.Clear();
        uboColors.Clear();
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