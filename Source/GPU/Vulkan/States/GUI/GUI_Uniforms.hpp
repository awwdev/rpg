//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Objects/BufferExt.hpp"
#include "GPU/Vulkan/Objects/Descriptors.hpp"
#include "GPU/RenderData/RenderData.hpp"
#include "GPU/RenderData/_Old/RenderStructs.hpp"

#include "GUI/GUI_Base.hpp"
#include "Resources/CpuResources.hpp"
#include "GPU/Vulkan/_Old/Objects/ImageArray.hpp"

namespace rpg::gpu::vuk {

struct GUI_Uniforms
{
    UniformInfo infos [3];
    Descriptors descriptors;

    UniformBuffer<RenderData_GUI::UBO_Text, RenderData_GUI::UBO_TEXT_MAX> uboText;
    UniformBuffer<RenderData_GUI::UBO_ColorTable, 1> uboColorTable;
    VkSampler  sampler;
    ImageArray fontImages;

    void Create(VkCommandPool cmdPool, res::CpuResources& hostRes)
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

        uboColorTable.Create();
        RenderData_GUI::UBO_ColorTable colors;
        std::memcpy(colors.colors, gui::ColorValues.data, gui::ColorValues.ENUM_END * sizeof(com::Vec4f));
        uboColorTable.Append(colors);
        uboColorTable.Bake(cmdPool);
        
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
                .buffer = uboColorTable.activeBuffer->buffer,
                .offset = 0,
                .range  = VK_WHOLE_SIZE
            }
        };

        //? DESCRIPTORS
        descriptors.Create(infos);
    }

    void Update(const com::Array<RenderData_GUI::UBO_Text, RenderData_GUI::UBO_TEXT_MAX>& pUboText)
    {
        uboText.Reset();
        uboText.Append(pUboText.Data(), pUboText.Count());
    }

    void Destroy()
    {
        uboText.Destroy();
        uboColorTable.Destroy();
        vkDestroySampler(g_contextPtr->device, sampler, nullptr);
        descriptors.Destroy();
        fontImages.Destroy();
        FOR_CARRAY(infos, i)
            infos[i] = {};
    }
    ~GUI_Uniforms()
    {
        Destroy();
    }

};

}//ns