//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Meta/Context.hpp"

namespace rpg::gpu::vuk {

struct UniformInfo2
{
    enum Type { Buffer, Image } type;
    VkDescriptorSetLayoutBinding binding {}; //includes binding
    //union {
        VkDescriptorBufferInfo bufferInfo {};
        VkDescriptorImageInfo  imageInfo  {};
    //};
};

struct Descriptors
{
    //capacity
    VkDescriptorSetLayout        descSetLayout;
    VkDescriptorPool             descPool;
    VkArray<VkDescriptorSet, 10> descSets;

    template<auto UNIFORM_COUNT>
    void Create(UniformInfo2 (&uniformInfos) [UNIFORM_COUNT])
    {
        //? LAYOUT
        VkDescriptorSetLayoutBinding bindings [UNIFORM_COUNT];
        for(uint32_t i = 0; i < UNIFORM_COUNT; ++i) {
            bindings[i] = uniformInfos[i].binding;
        }
        const auto descSetLayoutInfo = DescSetLayoutInfo(ArrayCount(bindings), bindings);
        VkCheck(vkCreateDescriptorSetLayout(g_contextPtr->device, &descSetLayoutInfo, nullptr, &descSetLayout));

        //? POOL
        VkDescriptorPoolSize poolSizes [UNIFORM_COUNT];
        for(uint32_t i = 0; i < UNIFORM_COUNT; ++i) {
            poolSizes[i] = {
                .type = bindings[i].descriptorType,
                .descriptorCount = 1
            };
        }
        const uint32_t MAX_SETS = 1; //arbitrary, max swapchain images
        const auto descPoolInfo = DescPoolInfo(MAX_SETS, ArrayCount(poolSizes), poolSizes); 
        VkCheck(vkCreateDescriptorPool(g_contextPtr->device, &descPoolInfo, nullptr, &descPool));

        //? ALLOCATION
        descSets.count = 1;
        const auto allocInfo = DescSetAllocInfo(descPool, 1, &descSetLayout);
        VkCheck(vkAllocateDescriptorSets(g_contextPtr->device, &allocInfo, descSets.data));

        //? WRITE
        VkWriteDescriptorSet writes [UNIFORM_COUNT];
        FOR_C_ARRAY(writes, i)
        {
            writes[i] = VkWriteDescriptorSet {
                .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext              = nullptr,
                .dstSet             = descSets.data[0],
                .dstBinding         = uniformInfos[i].binding.binding,
                .dstArrayElement    = 0,
                .descriptorCount    = 1,
                .descriptorType     = uniformInfos[i].binding.descriptorType,
                .pImageInfo         = uniformInfos[i].type == UniformInfo2::Image  ? &uniformInfos[i].imageInfo  : nullptr,
                .pBufferInfo        = uniformInfos[i].type == UniformInfo2::Buffer ? &uniformInfos[i].bufferInfo : nullptr,
                .pTexelBufferView   = nullptr
            };
        }
        vkUpdateDescriptorSets(g_contextPtr->device, ArrayCount(writes), writes, 0, nullptr);   
    }

    void Destroy()
    {
        if (descSetLayout != VK_NULL_HANDLE){
            vkDestroyDescriptorSetLayout(g_contextPtr->device, descSetLayout, nullptr);
            vkDestroyDescriptorPool(g_contextPtr->device, descPool, nullptr);
            descSets.count = 0;
        }
        descSetLayout = VK_NULL_HANDLE;
    }

    ~Descriptors()
    {
        Destroy();
    }

};

}//ns