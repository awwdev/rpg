//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Helper/Initializers.hpp"
#include "GPU/Vulkan/Helper/Utils.hpp"
#include "GPU/Vulkan/Objects/UniformBuffer.hpp"

#include "Common/Memory/Allocator.hpp"
#include <fstream>

namespace rpg::gpu::vuk {

template<auto BUFFER_SIZE = 8000u>
void CreateShaderModule(
chars_t path, const VkShaderStageFlagBits stage,
VkShaderModule& mod, VkPipelineShaderStageCreateInfo& stageInfo)
{
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    rpg::dbg::Assert(file.is_open(), "cannot open shader file");

    //com::mem::ClaimBlock<int[10]>();
    //struct Arr { char data[BUFFER_SIZE]; };
    const uint32_t size = (uint32_t)file.tellg();
    auto ptrBuffer = com::mem::ClaimBlock<char[BUFFER_SIZE]>();
    file.seekg(std::ios::beg);
    file.read(*ptrBuffer, size);

    //! SOMETHING IS WRONG WITH THE MEMORY ALLOACTOR!

    //const uint32_t size = (uint32_t)file.tellg();
    //char buffer [BUFFER_SIZE];
    //file.seekg(std::ios::beg);
    //file.read(buffer, size);

    const VkShaderModuleCreateInfo moduleInfo {
        .sType      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext      = nullptr,
        .flags      = 0,
        .codeSize   = size,
        .pCode      = reinterpret_cast<const uint32_t*>(*ptrBuffer)
        //.pCode      = reinterpret_cast<const uint32_t*>(buffer)
    };
    VkCheck(vkCreateShaderModule(g_contextPtr->device, &moduleInfo, nullptr, &mod));

    stageInfo = VkPipelineShaderStageCreateInfo {
        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext  = nullptr,
        .flags  = 0,
        .stage  = stage,
        .module = mod,
        .pName  = "main",
        .pSpecializationInfo = nullptr 
    };
}

template<uint32_t UNIFORM_COUNT>
void CreateDescriptors(
UniformInfo (&uniformInfos) [UNIFORM_COUNT],
VkDescriptorSetLayout* descSetLayouts, const uint32_t descSetLayoutCount,
VkDescriptorPool descPool,
VkDescriptorSet* descSets)
{
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
        VkCheck(vkCreateDescriptorSetLayout(g_contextPtr->device, &descSetLayoutInfo, nullptr, &descSetLayouts[i]));

    const auto descPoolInfo = DescPoolInfo(descSetLayoutCount, UNIFORM_COUNT, poolSizes);
    VkCheck(vkCreateDescriptorPool(g_contextPtr->device, &descPoolInfo, nullptr, &descPool));

    const auto allocInfo = DescSetAllocInfo(descPool, descSetLayoutCount, descSetLayouts);
    VkCheck(vkAllocateDescriptorSets(g_contextPtr->device, &allocInfo, descSets));

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

}//ns