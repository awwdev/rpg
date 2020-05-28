//https://github.com/awwdev

#pragma once
#include "mini/Debug/Assert.hpp"
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Ctors.hpp"
#include "mini/Vulkan/Objects/Image.hpp"

#include "mini/Resources/ResourceManager.hpp"
#include "mini/Memory/Allocator.hpp"
#include "mini/Box/SimpleArray.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Box/Map.hpp"


namespace mini::vk
{
    struct Refs
    {
        VkDevice device;
        Refs(VkDevice pDevice) : device { pDevice } {;}
    };

    struct Shader : Refs
    {
        //stages
        box::Array<VkShaderModule, 4> modules;
        box::Array<VkPipelineShaderStageCreateInfo, 4> stageInfos;

        //vertex
        box::Array<VkVertexInputBindingDescription, 10>   vertBindings;
        box::Array<VkVertexInputAttributeDescription, 10> vertAttributes;

        //uniform
        VkDescriptorPool descPool;
        box::Array<VkSampler, 4> samplers;
        box::Map<VkDescriptorImageInfo, 4>  imageInfos;
        box::Map<VkDescriptorBufferInfo, 4> bufferInfos;

        box::Array<VkDescriptorSetLayoutBinding, 4> setLayoutBindings;
        box::Array<VkDescriptorSetLayout, 10> setLayouts;
        box::Array<VkDescriptorSet, 10> sets;

        ~Shader()
        {
            vkDestroyDescriptorPool(device, descPool, nullptr);
            FOR_ARRAY(modules, i)    vkDestroyShaderModule(device, modules[i], nullptr);
            FOR_ARRAY(samplers, i)   vkDestroySampler(device, samplers[i], nullptr);
            FOR_ARRAY(setLayouts, i) vkDestroyDescriptorSetLayout(device, setLayouts[i], nullptr); 
        }
    };

    template<auto BUFFER_SIZE = 10000u>
    void CreateShaderModule(Shader& shader, chars_t path, const VkShaderStageFlagBits stage)
    {
        std::ifstream file(path, std::ios::ate | std::ios::binary);
        mini::Assert(file.is_open(), "cannot open shader file");

        const uint32_t size = file.tellg();
        auto ptrBuffer = mem::ClaimBlock<char[BUFFER_SIZE]>();
        file.seekg(std::ios::beg);
        file.read(*ptrBuffer, size);

        const VkShaderModuleCreateInfo moduleInfo {
            .sType      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext      = nullptr,
            .flags      = 0,
            .codeSize   = size,
            .pCode      = reinterpret_cast<const uint32_t*>(*ptrBuffer)
        };

        auto& mod = shader.modules.AppendReturn();
        VK_CHECK(vkCreateShaderModule(shader.device, &moduleInfo, nullptr, &mod));

        shader.stageInfos.Append(VkPipelineShaderStageCreateInfo{
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext  = nullptr,
            .flags  = 0,
            .stage  = stage,
            .module = mod,
            .pName  = "main",
            .pSpecializationInfo = nullptr 
        });
    }

    inline void CreateDescriptors(Context& context, Shader& shader)
    {
        const VkDescriptorSetLayoutCreateInfo setLayoutInfo
        {
            .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext          = nullptr,
            .flags          = 0,
            .bindingCount   = shader.setLayoutBindings.Count(),
            .pBindings      = shader.setLayoutBindings.Data()
        };

        for(uint32_t i = 0; i < context.swapImages.count; ++i)
            VK_CHECK(vkCreateDescriptorSetLayout(context.device, &setLayoutInfo, nullptr, &shader.setLayouts.AppendReturn()));

        box::Array<VkDescriptorPoolSize, 10> poolSizes;
        for(uint32_t i = 0; i < shader.setLayoutBindings.Count(); ++i) {
            poolSizes.Append(VkDescriptorPoolSize{
                .type = shader.setLayoutBindings[i].descriptorType,
                .descriptorCount = context.swapImages.count
            });
        }

        const VkDescriptorPoolCreateInfo poolInfo {
            .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext          = nullptr,
            .flags          = 0,
            .maxSets        = context.swapImages.count,
            .poolSizeCount  = poolSizes.Count(),
            .pPoolSizes     = poolSizes.Data()
        };
        VK_CHECK(vkCreateDescriptorPool(context.device, &poolInfo, nullptr, &shader.descPool));

        const VkDescriptorSetAllocateInfo allocInfo {
            .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext              = nullptr,
            .descriptorPool     = shader.descPool,
            .descriptorSetCount = shader.setLayouts.Count(),
            .pSetLayouts        = shader.setLayouts.Data()
        };
        shader.sets.count = shader.setLayouts.Count();
        VK_CHECK(vkAllocateDescriptorSets(context.device, &allocInfo, shader.sets.Data()));

        box::Array<VkWriteDescriptorSet, 10> writes;
        for(auto i = 0; i < context.swapImages.count; ++i)
        {
            FOR_ARRAY(shader.setLayoutBindings, j)
            {
                writes.Append(VkWriteDescriptorSet{
                    .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .pNext              = nullptr,
                    .dstSet             = shader.sets[i],
                    .dstBinding         = j,
                    .dstArrayElement    = 0,
                    .descriptorCount    = 1,
                    .descriptorType     = shader.setLayoutBindings[j].descriptorType,
                    .pImageInfo         = shader.imageInfos.GetOptional(j),
                    .pBufferInfo        = shader.bufferInfos.GetOptional(j),
                    .pTexelBufferView   = nullptr
                });
            }
        }
        vkUpdateDescriptorSets(context.device, writes.Count(), writes.Data(), 0, nullptr);   
    }

    inline void CreateShader_Default(Context& context, Shader& shader, Image images[])
    {  
        CreateShaderModule(shader, "res/Shaders/default.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
        CreateShaderModule(shader, "res/Shaders/default.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

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

        auto& image = images[res::Font];
        shader.imageInfos.Set(0, VkDescriptorImageInfo{
            .sampler        = shader.samplers[0],
            .imageView      = image.view, 
            .imageLayout    = image.layout
        });       
 
        CreateDescriptors(context, shader);
    }


}//ns