//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Objects/Shader.hpp"
#include "mini/Vulkan/Objects/UniformBuffer.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/Utils/Vertex.hpp"

namespace mini::vk
{
    inline void CreateShader_Default(Context& context, Shader& shader, Image images[], UniformBuffer<bool>& ubo) //or pass some upper struct UBOS
    {  
        shader.CreateShaderModule("res/Shaders/default.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
        shader.CreateShaderModule("res/Shaders/default.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

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

        //TODO: when we have ubo we will move this stuff out too ?

        shader.setLayoutBindings.Append(VkDescriptorSetLayoutBinding{
            .binding            = 0,
            .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount    = 1,
            .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
            .pImmutableSamplers = nullptr,
        }); 
        shader.setLayoutBindings.Append(VkDescriptorSetLayoutBinding{
            .binding            = 1,
            .descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
            .descriptorCount    = 1,
            .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
            .pImmutableSamplers = nullptr,
        }); 

        auto& image = images[hostRes::Font];
        shader.imageInfos.Set(0, VkDescriptorImageInfo{
            .sampler        = shader.samplers[0],
            .imageView      = image.view, 
            .imageLayout    = image.layout
        });      

        shader.bufferInfos.Set(1, VkDescriptorBufferInfo{
            //get infos from the very buffer
            .buffer = ubo.buffer.buffer,
            .offset = 0, 
            .range  = VK_WHOLE_SIZE
        });   
 
        shader.CreateDescriptors(context);
    }

}//ns