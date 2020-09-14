//https://github.com/awwdev

#pragma once

#include "GPU/Vulkan/Meta/Context.hpp"

namespace rpg::gpu::vuk {

struct Commands
{
    VkCommandPool cmdPool;
    vuk::VkArray<VkCommandBuffer, 4> cmdBuffers { 0 };

    void Create()
    {
        //? CMD POOL
        const VkCommandPoolCreateInfo poolInfo
        {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = g_contextPtr->queueIndex
        };
        VkCheck(vkCreateCommandPool(g_contextPtr->device, &poolInfo, nullptr, &cmdPool));

        //? CMD BUFFERS

        cmdBuffers.count = g_contextPtr->swapImages.count;
        const VkCommandBufferAllocateInfo allocInfo
        {
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext              = nullptr,
            .commandPool        = cmdPool,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = cmdBuffers.count
        };
        VkCheck(vkAllocateCommandBuffers(g_contextPtr->device, &allocInfo, cmdBuffers.data));
    }

    ~Commands()
    {
        Clear();
    }

    void Clear()
    {
        vkFreeCommandBuffers(g_contextPtr->device, cmdPool, cmdBuffers.count, cmdBuffers.data);
        vkDestroyCommandPool(g_contextPtr->device, cmdPool, nullptr);
    }
};

inline VkCommandBufferBeginInfo CreateCmdBeginInfo(const VkCommandBufferUsageFlags flags = 0)
{
    return {
        .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext            = nullptr,
        .flags            = flags,
        .pInheritanceInfo = nullptr
    };
}

inline VkCommandBuffer BeginCommands_OneTime(VkDevice device, VkCommandPool cmdPool)
{
    VkCommandBuffer commandBuffer;

    const VkCommandBufferAllocateInfo allocInfo {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext              = nullptr,
        .commandPool        = cmdPool,
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };
    VkCheck(vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer));

    const auto beginInfo = CreateCmdBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    VkCheck(vkBeginCommandBuffer(commandBuffer, &beginInfo));
    
    return commandBuffer;
}


inline void EndCommands_OneTime(
    VkDevice device, 
    VkCommandBuffer cmdBuffer, 
    VkCommandPool cmdPool,
    VkQueue queue)
{
    VkCheck(vkEndCommandBuffer(cmdBuffer));

    const VkSubmitInfo submitInfo {
        .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext                = nullptr,
        .waitSemaphoreCount   = 0,
        .pWaitSemaphores      = nullptr,
        .pWaitDstStageMask    = nullptr,
        .commandBufferCount   = 1,
        .pCommandBuffers      = &cmdBuffer,
        .signalSemaphoreCount = 0,
        .pSignalSemaphores    = nullptr
    };

    VkCheck(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
    VkCheck(vkQueueWaitIdle(queue));
    vkFreeCommandBuffers(device, cmdPool, 1, &cmdBuffer);
}

} //ns