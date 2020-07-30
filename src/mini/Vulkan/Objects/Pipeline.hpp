//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Box/Array2.hpp"

namespace mini::vk
{
    struct Pipeline
    {
        VkPipeline pipeline;
        VkPipelineLayout layout;

        VkDescriptorPool descPool;
        box2::Array<VkDescriptorSetLayout, 10> setLayouts;
        box2::Array<VkDescriptorSet, 10> sets;

        ~Pipeline()
        {
            vkDestroyPipelineLayout (g_contextPtr->device, layout, nullptr);
            vkDestroyPipeline       (g_contextPtr->device, pipeline, nullptr);
            vkDestroyDescriptorPool (g_contextPtr->device, descPool, nullptr);
            FOR_ARRAY2(setLayouts, i) 
                vkDestroyDescriptorSetLayout(g_contextPtr->device, setLayouts[i], nullptr); 
        }
    };

    //TODO: do recreate on user side ... find a way

    template<std::size_t N>
    void WriteDescriptors(
        Pipeline& pipeline,
        UniformInfo* (&uniformInfos)[N])
    {
        VkDescriptorSetLayoutBinding bindings [N];
        const uint32_t bindingsCount = N;
        for(auto i = 0; i < bindingsCount; ++i) {
            bindings[i] = uniformInfos[i]->layout;
        }

        const VkDescriptorSetLayoutCreateInfo setLayoutInfo
        {
            .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext          = nullptr,
            .flags          = 0,
            .bindingCount   = N,
            .pBindings      = bindings
        };

        for(uint32_t i = 0; i < g_contextPtr->swapImages.count; ++i)
            VK_CHECK(vkCreateDescriptorSetLayout(g_contextPtr->device, &setLayoutInfo, nullptr, pipeline.setLayouts.Append()));

        box2::Array<VkDescriptorPoolSize, 10> poolSizes;
        for(uint32_t i = 0; i < bindingsCount; ++i) {
            poolSizes.Append(VkDescriptorPoolSize{
                .type = bindings[i].descriptorType,
                .descriptorCount = g_contextPtr->swapImages.count
            });
        }

        const VkDescriptorPoolCreateInfo poolInfo {
            .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext          = nullptr,
            .flags          = 0,
            .maxSets        = g_contextPtr->swapImages.count,// * poolSizes.Count(), //! needed ? 
            .poolSizeCount  = (u32)poolSizes.count,
            .pPoolSizes     = &poolSizes[0]
        };
        VK_CHECK(vkCreateDescriptorPool(g_contextPtr->device, &poolInfo, nullptr, &pipeline.descPool));

        const VkDescriptorSetAllocateInfo allocInfo {
            .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext              = nullptr,
            .descriptorPool     = pipeline.descPool,
            .descriptorSetCount = (u32)pipeline.setLayouts.count,
            .pSetLayouts        = &pipeline.setLayouts[0]
        };
        pipeline.sets.count = pipeline.setLayouts.count;
        VK_CHECK(vkAllocateDescriptorSets(g_contextPtr->device, &allocInfo, &pipeline.sets[0]));

        box2::Array<VkWriteDescriptorSet, 10> writes;
        for(u32 i = 0; i < g_contextPtr->swapImages.count; ++i)
        {
            FOR_CARRAY(uniformInfos, j)
            {
                writes.Append(VkWriteDescriptorSet{
                    .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .pNext              = nullptr,
                    .dstSet             = pipeline.sets[i],
                    .dstBinding         = uniformInfos[j]->layout.binding,
                    .dstArrayElement    = 0,
                    .descriptorCount    = 1,
                    .descriptorType     = uniformInfos[j]->layout.descriptorType,
                    .pImageInfo         = uniformInfos[j]->type == UniformInfo::Image  ? &uniformInfos[j]->imageInfo  : nullptr,
                    .pBufferInfo        = uniformInfos[j]->type == UniformInfo::Buffer ? &uniformInfos[j]->bufferInfo : nullptr,
                    .pTexelBufferView   = nullptr
                });
            }
        }
        vkUpdateDescriptorSets(g_contextPtr->device, (u32)writes.count, &writes[0], 0, nullptr);   
    }

}//ns
