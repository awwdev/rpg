//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Utils/Vertex.hpp"
#include "mini/Box/Array.hpp"

namespace mini::vk
{
    struct Pipeline
    {
        VkDevice device;

        VkPipeline pipeline;
        VkPipelineLayout layout;

        VkDescriptorPool descPool;
        box::Array<VkDescriptorSetLayout, 10> setLayouts;
        box::Array<VkDescriptorSet, 10> sets;

        explicit Pipeline(VkDevice pDevice) : device { pDevice } {;}

        ~Pipeline()
        {
            vkDestroyPipelineLayout(device, layout, nullptr);
            vkDestroyPipeline(device, pipeline, nullptr);
            vkDestroyDescriptorPool(device, descPool, nullptr);
            FOR_ARRAY(setLayouts, i) 
                vkDestroyDescriptorSetLayout(device, setLayouts[i], nullptr); 
        }
    };

    //TODO: do recreate on user side ... find a way

    //! MOST CONFUSING PART OF BASE VULKAN:
    template<class... DESCRIPTORS>
    void WriteDescriptors(
        Context& context, 
        Pipeline& pipeline,
        const DESCRIPTORS&... descs)
    {
        const VkDescriptorSetLayoutBinding bindings [] = { descs... };

        const VkDescriptorSetLayoutCreateInfo setLayoutInfo
        {
            .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext          = nullptr,
            .flags          = 0,
            .bindingCount   = sizeof...(DESCRIPTORS),
            .pBindings      = bindings
        };

        for(uint32_t i = 0; i < context.swapImages.count; ++i)
            VK_CHECK(vkCreateDescriptorSetLayout(context.device, &setLayoutInfo, nullptr, &pipeline.setLayouts.AppendReturn()));

        box::Array<VkDescriptorPoolSize, 10> poolSizes;
        for(uint32_t i = 0; i < sizeof...(DESCRIPTORS); ++i) {
            poolSizes.Append(VkDescriptorPoolSize{
                .type = bindings[i].descriptorType,
                .descriptorCount = context.swapImages.count
            });
        }

        const VkDescriptorPoolCreateInfo poolInfo {
            .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext          = nullptr,
            .flags          = 0,
            .maxSets        = context.swapImages.count,// * poolSizes.Count(), //! needed ? 
            .poolSizeCount  = poolSizes.Count(),
            .pPoolSizes     = poolSizes.Data()
        };
        VK_CHECK(vkCreateDescriptorPool(context.device, &poolInfo, nullptr, &pipeline.descPool));

        const VkDescriptorSetAllocateInfo allocInfo {
            .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext              = nullptr,
            .descriptorPool     = pipeline.descPool,
            .descriptorSetCount = pipeline.setLayouts.Count(),
            .pSetLayouts        = pipeline.setLayouts.Data()
        };
        pipeline.sets.count = pipeline.setLayouts.Count();
        VK_CHECK(vkAllocateDescriptorSets(context.device, &allocInfo, pipeline.sets.Data()));

        box::Array<VkWriteDescriptorSet, 10> writes;
        for(auto i = 0; i < context.swapImages.count; ++i)
        {
            FOR_CARRAY(bindings, j)
            {
                writes.Append(VkWriteDescriptorSet{
                    .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .pNext              = nullptr,
                    .dstSet             = pipeline.sets[i],
                    .dstBinding         = (uint32_t)j,
                    .dstArrayElement    = 0,
                    .descriptorCount    = 1,
                    .descriptorType     = bindings[j].descriptorType,
                    //! HERE MY FUNCTION DOES NOT WORK ANYMORE
                    //WE NEED MORE ELABORATED PARAMS TO PASS LIKE A MAP
                    .pImageInfo         = nullptr, //imageInfos.GetValueOptional(j),
                    .pBufferInfo        = nullptr, //bufferInfos.GetValueOptional(j),
                    .pTexelBufferView   = nullptr
                });
            }
        }
        vkUpdateDescriptorSets(context.device, writes.Count(), writes.Data(), 0, nullptr);   
    }

}//ns
