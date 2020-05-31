//https://github.com/awwdev

#pragma once
#include "mini/Debug/Assert.hpp"
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Ctors.hpp"
#include "mini/Vulkan/Objects/Image.hpp"

#include "mini/Memory/Allocator.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Box/Map.hpp"

namespace mini::vk
{
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


        template<auto BUFFER_SIZE = 10000u>
        void CreateShaderModule(chars_t path, const VkShaderStageFlagBits stage)
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

            auto& mod = modules.AppendReturn();
            VK_CHECK(vkCreateShaderModule(device, &moduleInfo, nullptr, &mod));

            stageInfos.Append(VkPipelineShaderStageCreateInfo{
                .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .pNext  = nullptr,
                .flags  = 0,
                .stage  = stage,
                .module = mod,
                .pName  = "main",
                .pSpecializationInfo = nullptr 
            });
        }

        inline void CreateDescriptors(Context& context)
        {
            const VkDescriptorSetLayoutCreateInfo setLayoutInfo
            {
                .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                .pNext          = nullptr,
                .flags          = 0,
                .bindingCount   = setLayoutBindings.Count(),
                .pBindings      = setLayoutBindings.Data()
            };

            for(uint32_t i = 0; i < context.swapImages.count; ++i)
                VK_CHECK(vkCreateDescriptorSetLayout(context.device, &setLayoutInfo, nullptr, &setLayouts.AppendReturn()));

            box::Array<VkDescriptorPoolSize, 10> poolSizes;
            for(uint32_t i = 0; i < setLayoutBindings.Count(); ++i) {
                poolSizes.Append(VkDescriptorPoolSize{
                    .type = setLayoutBindings[i].descriptorType,
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
            VK_CHECK(vkCreateDescriptorPool(context.device, &poolInfo, nullptr, &descPool));

            const VkDescriptorSetAllocateInfo allocInfo {
                .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                .pNext              = nullptr,
                .descriptorPool     = descPool,
                .descriptorSetCount = setLayouts.Count(),
                .pSetLayouts        = setLayouts.Data()
            };
            sets.count = setLayouts.Count();
            VK_CHECK(vkAllocateDescriptorSets(context.device, &allocInfo, sets.Data()));

            box::Array<VkWriteDescriptorSet, 10> writes;
            for(auto i = 0; i < context.swapImages.count; ++i)
            {
                FOR_ARRAY(setLayoutBindings, j)
                {
                    writes.Append(VkWriteDescriptorSet{
                        .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                        .pNext              = nullptr,
                        .dstSet             = sets[i],
                        .dstBinding         = j,
                        .dstArrayElement    = 0,
                        .descriptorCount    = 1,
                        .descriptorType     = setLayoutBindings[j].descriptorType,
                        .pImageInfo         = imageInfos.GetOptional(j),
                        .pBufferInfo        = bufferInfos.GetOptional(j),
                        .pTexelBufferView   = nullptr
                    });
                }
            }
            vkUpdateDescriptorSets(context.device, writes.Count(), writes.Data(), 0, nullptr);   
        }
    };

}//ns