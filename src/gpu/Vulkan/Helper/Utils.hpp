//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/Helper/Initializers.hpp"
#include "gpu/Vulkan/Helper/Utils.hpp"

#include "com/mem/Allocator.hpp"
#include <fstream>

namespace rpg::gpu::vuk {

struct VertexRange { uint32_t index, count; };

template<auto BUFFER_SIZE = 8000u>
void CreateShaderModule(
chars_t path, const VkShaderStageFlagBits stage,
VkShaderModule& mod, VkPipelineShaderStageCreateInfo& stageInfo)
{
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    rpg::dbg::Assert(file.is_open(), "cannot open shader file");

    const auto size = (uint32_t)file.tellg();
    struct Arr { char data [BUFFER_SIZE]; };
    auto  ptrBufferArr = com::mem::ClaimBlock<Arr>();
    auto& buffer = ptrBufferArr->data;
    file.seekg(std::ios::beg);
    file.read(buffer, size);

    const VkShaderModuleCreateInfo moduleInfo {
        .sType      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext      = nullptr,
        .flags      = 0,
        .codeSize   = size,
        .pCode      = reinterpret_cast<const uint32_t*>(buffer)
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

inline uint32_t MemoryType(
    const VkPhysicalDeviceMemoryProperties& physicalMemProps,
    const VkMemoryRequirements& memReqs,
    const VkMemoryPropertyFlags neededMemProps) 
{
    for (uint32_t i = 0; i < physicalMemProps.memoryTypeCount; ++i) {
        if (memReqs.memoryTypeBits & (1 << i) &&
            (physicalMemProps.memoryTypes[i].propertyFlags & neededMemProps) == neededMemProps) 
            return i;
    }
    dbg::LogError("no suitable memory type found!");
    return {};
}

inline VkMemoryAllocateInfo MemoryAllocInfo(const VkDeviceSize& size, const uint32_t memTypeIndex)
{
    return {
        .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext           = nullptr,
        .allocationSize  = size,
        .memoryTypeIndex = memTypeIndex
    };
}

inline auto BufferAllocInfo(VkCommandPool cmdPool, const uint32_t count = 1)
{
    return VkCommandBufferAllocateInfo {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext              = nullptr,
        .commandPool        = cmdPool,
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = count
    };
}

inline auto CmdBufferBeginInfo(const VkCommandBufferUsageFlags flags = 0)
{
    return VkCommandBufferBeginInfo {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext              = 0,
        .flags              = flags,
        .pInheritanceInfo   = 0,
    };
}

inline auto CopyBuffer(VkCommandPool cmdPool, const VkBuffer src, VkBuffer dst, const std::size_t size)
{
    //create temp command buffer
    const auto allocInfo = BufferAllocInfo(cmdPool);
    VkCommandBuffer commandBuffer;
    VkCheck(vkAllocateCommandBuffers(g_contextPtr->device, &allocInfo, &commandBuffer));

    //record the command
    const auto beginInfo = CmdBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    VkCheck(vkBeginCommandBuffer(commandBuffer, &beginInfo));
    const VkBufferCopy copyRegion {
        .srcOffset = 0,
        .dstOffset = 0,
        .size = size
    };
    vkCmdCopyBuffer(commandBuffer, src, dst, 1, &copyRegion);
    VkCheck(vkEndCommandBuffer(commandBuffer));

    //submit
    const VkSubmitInfo submitInfo {
        .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext                  = 0,
        .waitSemaphoreCount     = 0,
        .pWaitSemaphores        = 0,
        .pWaitDstStageMask      = 0,
        .commandBufferCount     = 1,
        .pCommandBuffers        = &commandBuffer,
        .signalSemaphoreCount   = 0,
        .pSignalSemaphores      = 0
    };
    VkCheck(vkQueueSubmit(g_contextPtr->queue, 1, &submitInfo, VK_NULL_HANDLE));
    VkCheck(vkQueueWaitIdle(g_contextPtr->queue));

    vkFreeCommandBuffers(g_contextPtr->device, cmdPool, 1, &commandBuffer);
}

}//ns