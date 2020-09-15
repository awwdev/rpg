//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Objects/UniformBuffer.hpp"

namespace rpg::gpu::vuk {

struct Descriptors
{
    VkArray<VkDescriptorSetLayout, 4>   descSetLayouts;
    VkArray<VkDescriptorSet, 4>         descSets;
    VkDescriptorPool                    descPool;

    template<auto N>
    void Create(UniformInfo (&uniformInfos) [N])
    {
        descSetLayouts.count = descSets.count = g_contextPtr->swapImages.count;
        constexpr auto UNIFORM_COUNT = 1;
        const auto descSetLayoutCount = descSetLayouts.count;

        VkDescriptorSetLayoutBinding bindings [UNIFORM_COUNT];
        VkDescriptorPoolSize poolSizes [UNIFORM_COUNT];

        for(uint32_t i = 0; i < UNIFORM_COUNT; ++i) {
            bindings[i]  = uniformInfos[i].layout;
            poolSizes[i] = {
                .type = bindings[i].descriptorType,
                .descriptorCount = descSetLayoutCount
            };
        }

        const auto descSetLayoutInfo = DescSetLayoutInfo(bindings, UNIFORM_COUNT);
        for(uint32_t i = 0; i < descSetLayoutCount; ++i) 
            VkCheck(vkCreateDescriptorSetLayout(g_contextPtr->device, &descSetLayoutInfo, nullptr, &(descSetLayouts[i])));

        const auto descPoolInfo = DescPoolInfo(descSetLayoutCount, UNIFORM_COUNT, poolSizes);
        VkCheck(vkCreateDescriptorPool(g_contextPtr->device, &descPoolInfo, nullptr, &descPool));

        const auto allocInfo = DescSetAllocInfo(descPool, descSetLayoutCount, descSetLayouts.data);
        VkCheck(vkAllocateDescriptorSets(g_contextPtr->device, &allocInfo, descSets.data));

        VkArray<VkWriteDescriptorSet, 10> writes;
        writes.count = descSetLayoutCount * UNIFORM_COUNT;
        for(u32 i = 0; i < descSetLayoutCount; ++i)
        {
            FOR_CARRAY(uniformInfos, j)
            {
                writes[i * UNIFORM_COUNT + j] = {
                    .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .pNext              = nullptr,
                    .dstSet             = descSets[i],
                    .dstBinding         = uniformInfos[j].layout.binding,
                    .dstArrayElement    = 0,
                    .descriptorCount    = 1,
                    .descriptorType     = uniformInfos[j].layout.descriptorType,
                    .pImageInfo         = uniformInfos[j].type == UniformInfo::Image  ? &uniformInfos[j].imageInfo  : nullptr,
                    .pBufferInfo        = uniformInfos[j].type == UniformInfo::Buffer ? &uniformInfos[j].bufferInfo : nullptr,
                    .pTexelBufferView   = nullptr
                };
            }
        }
        vkUpdateDescriptorSets(g_contextPtr->device, writes.count, writes.data, 0, nullptr);   
        
    }

    void Clear()
    {
        FOR_VK_ARRAY(descSetLayouts, i)
            vkDestroyDescriptorSetLayout(g_contextPtr->device, descSetLayouts[i], nullptr);
        descSetLayouts.count = 0;
        descSets.count = 0;
        vkDestroyDescriptorPool(g_contextPtr->device, descPool, nullptr);
    }

    ~Descriptors()
    {
        Clear();
    }

};

}//ns