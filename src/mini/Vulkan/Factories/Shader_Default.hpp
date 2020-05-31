//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Objects/Shader.hpp"
#include "mini/Resources/ResourceManager.hpp"

namespace mini::vk
{
    inline void CreateShader_Default(Context& context, Shader& shader, Image images[])
    {  
        shader.CreateShaderModule("res/Shaders/default.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
        shader.CreateShaderModule("res/Shaders/default.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

        shader.vertBindings.Append(VkVertexInputBindingDescription{
            .binding    = 0,
            .stride     = sizeof(float) * 2,
            .inputRate  = VK_VERTEX_INPUT_RATE_VERTEX
        });

        shader.vertAttributes.Append(VkVertexInputAttributeDescription{
            .location   = 0,
            .binding    = 0,
            .format     = VK_FORMAT_R32_SFLOAT,
            .offset     = sizeof(float) * 0,
        });
        shader.vertAttributes.Append(VkVertexInputAttributeDescription{
            .location   = 1,
            .binding    = 0,
            .format     = VK_FORMAT_R32_SFLOAT,
            .offset     = sizeof(float) * 1,
        });

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

        shader.setLayoutBindings.Append(VkDescriptorSetLayoutBinding{
            .binding            = 0,
            .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount    = 1,
            .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
            .pImmutableSamplers = nullptr,
        }); 

        auto& image = images[res::Texture1];
        shader.imageInfos.Set(0, VkDescriptorImageInfo{
            .sampler        = shader.samplers[0],
            .imageView      = image.view, 
            .imageLayout    = image.layout
        });       
 
        shader.CreateDescriptors(context);
    }

}//ns

//!old dedicated struct
/*
namespace mini::vk
{
    struct Default_Shader
    {
        VkDevice device;
        
        VkSampler sampler;
        VkDescriptorPool descPool;
        box::SimpleArray<VkDescriptorSetLayout, 4> layouts { 0 };
        box::SimpleArray<VkDescriptorSet, 4>       sets    { 0 };

        const VkVertexInputBindingDescription BINDING_DESCS [1] 
        {
            {
                .binding    = 0,
                .stride     = sizeof(float) * 2, //!fill vertex
                .inputRate  = VK_VERTEX_INPUT_RATE_VERTEX
            },
        };

        const VkVertexInputAttributeDescription ATTRIBUTE_DESCS [2]
        {
            { //? position
                .location   = 0,
                .binding    = 0,
                .format     = VK_FORMAT_R32G32_SFLOAT,
                .offset     = sizeof(float) * 0, //!offset
            },
            { //? color
                .location   = 1,
                .binding    = 0,
                .format     = VK_FORMAT_R32G32B32A32_SFLOAT,
                .offset     = sizeof(float) * 1, //!offset
            }
        };

        VkPipelineShaderStageCreateInfo stages [2] //module will be filled in (ctor)
        {
            PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT),
            PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT)
        };



        inline void Create(Context& context, vk::Image images[])
        {
            device = context.device;
            auto& image = images[res::Font];

            //? SHADER MODULES

            stages[0].module = CreateShaderModule(device, "res/Shaders/default.vert.spv");
            stages[1].module = CreateShaderModule(device, "res/Shaders/default.frag.spv");

            //? SAMPLER

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

            VK_CHECK(vkCreateSampler(device, &samplerInfo, nullptr, &sampler));

            //? UNIFORMS

            const VkDescriptorSetLayoutBinding bindings [1] 
            {
                {
                    .binding            = 0,
                    .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    .descriptorCount    = 1,
                    .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
                    .pImmutableSamplers = nullptr,
                }
            }; 
            
            const VkDescriptorSetLayoutCreateInfo layoutSetInfo
            {
                .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                .pNext          = nullptr,
                .flags          = 0,
                .bindingCount   = ARRAY_COUNT(bindings),
                .pBindings      = bindings
            };

            layouts.count = context.swapImages.count;
            FOR_SIMPLE_ARRAY(layouts, i)
                VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutSetInfo, nullptr, &layouts[i]));

            VkDescriptorPoolSize poolSizes [ARRAY_COUNT(bindings)]; 
            FOR_CARRAY(bindings, i)
            {
                poolSizes[i] = {
                    .type = bindings[i].descriptorType,
                    .descriptorCount = context.swapImages.count
                };
            }

            const VkDescriptorPoolCreateInfo poolInfo
             {
                .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
                .pNext          = nullptr,
                .flags          = 0,
                .maxSets        = context.swapImages.count,
                .poolSizeCount  = ARRAY_COUNT(poolSizes),
                .pPoolSizes     = poolSizes
            };
            VK_CHECK(vkCreateDescriptorPool(device, &poolInfo, nullptr, &descPool));


            const VkDescriptorSetAllocateInfo allocInfo 
            {
                .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                .pNext              = nullptr,
                .descriptorPool     = descPool,
                .descriptorSetCount = layouts.count,
                .pSetLayouts        = layouts.data
            };
            sets.count = layouts.count;
            VK_CHECK(vkAllocateDescriptorSets(device, &allocInfo, sets.data));


            box::Array<VkWriteDescriptorSet, 10> writes;

            const VkDescriptorImageInfo imageInfo {
                .sampler        = sampler,
                .imageView      = image.view,
                .imageLayout    = image.layout
            };            

            for(auto i = 0; i < context.swapImages.count; ++i)
            {
                writes.Append(VkWriteDescriptorSet{
                    .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .pNext              = nullptr,
                    .dstSet             = sets[i],
                    .dstBinding         = 0,
                    .dstArrayElement    = 0,
                    .descriptorCount    = 1,
                    .descriptorType     = bindings[0].descriptorType,
                    .pImageInfo         = &imageInfo,
                    .pBufferInfo        = nullptr,
                    .pTexelBufferView   = nullptr
               });
            }
            vkUpdateDescriptorSets(device, writes.Count(), writes.Data(), 0, nullptr);
        }

        ~Default_Shader()
        {
            FOR_SIMPLE_ARRAY(layouts, i)
                vkDestroyDescriptorSetLayout(device, layouts[i], nullptr);
            vkDestroyDescriptorPool(device, descPool, nullptr);
            FOR_CARRAY(stages, i)
                vkDestroyShaderModule(device, stages[i].module, nullptr);
            vkDestroySampler(device, sampler, nullptr);
        }
    };

}//ns
*/