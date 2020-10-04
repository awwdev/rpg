//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/Objects/BufferExt.hpp"
#include "gpu/Vulkan/Objects/Descriptors.hpp"
#include "gpu/Vulkan/Objects/Image.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "gui/GUI_Base.hpp"
#include "res/Texture/TextureLoader.hpp"

namespace rpg::gpu::vuk {

struct GUI_Uniforms
{
    enum Bindings : uint32_t {
        BindingFontImages       = 0,
        BindingUboText          = 1,
        BindingColorTable       = 2,
        ENUM_END
    };

    UniformInfo infos [Bindings::ENUM_END];
    Descriptors descriptors;

    UniformBuffer<RenderData_GUI::UBO_Text, RenderData_GUI::UBO_TEXT_MAX> uboText;
    UniformBuffer<RenderData_GUI::UBO_ColorTable, 1> uboColorTable;
    VkSampler  sampler;
    Image      fontImages;

    void Create(VkCommandPool cmdPool, res::Resources& resources)
    {
        constexpr VkComponentMapping fontImagesComponentMapping = 
        { 
            .r = VK_COMPONENT_SWIZZLE_B,
            .g = VK_COMPONENT_SWIZZLE_G,
            .b = VK_COMPONENT_SWIZZLE_R,
            .a = VK_COMPONENT_SWIZZLE_A
        };

        fontImages.Create(
            VK_FORMAT_R8_SRGB,
            resources.textures.monospaceFont.WIDTH,
            resources.textures.monospaceFont.HEIGHT,
            VK_SAMPLE_COUNT_1_BIT, 
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_IMAGE_ASPECT_COLOR_BIT,
            resources.textures.monospaceFont.count,
            VK_IMAGE_VIEW_TYPE_2D_ARRAY,
            fontImagesComponentMapping
        );     
        fontImages.Store(
            cmdPool, 
            resources.textures.monospaceFont.data, 
            resources.textures.monospaceFont.ALL_TEXTURES_BYTES,
            resources.textures.monospaceFont.SINGLE_TEXTURE_BYTES
        );
        fontImages.Bake(cmdPool);

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

        infos[BindingFontImages] = {
            .type = UniformInfo::Image,
            .binding {
                .binding            = BindingFontImages,
                .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .descriptorCount    = 1,
                .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
                .pImmutableSamplers = nullptr,
            },
            .imageInfo {
                .sampler        = sampler,
                .imageView      = fontImages.view,
                .imageLayout    = fontImages.currentLayout
            }
        };

        uboText.Create();
        infos[BindingUboText] = {
            .type = UniformInfo::Buffer,
            .binding {
                .binding            = BindingUboText,
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
        
        infos[BindingColorTable] = {
            .type = UniformInfo::Buffer,
            .binding {
                .binding            = BindingColorTable,
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
        if (!pUboText.Empty())
             uboText.Append(pUboText);
    }

    void Destroy()
    {
        uboText.Destroy();
        uboColorTable.Destroy();
        vkDestroySampler(g_contextPtr->device, sampler, nullptr);
        descriptors.Destroy();
        fontImages.Destroy();
        FOR_C_ARRAY(infos, i)
            infos[i] = {};
    }
    ~GUI_Uniforms()
    {
        Destroy();
    }

};

}//ns