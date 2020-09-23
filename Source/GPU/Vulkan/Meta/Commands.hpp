//https://github.com/awwdev

#pragma once

#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/Helper/Initializers.hpp"

namespace rpg::gpu::vuk {

struct Commands
{
    VkCommandPool mainCmdPool; //for single time cmds on main thread

    static constexpr uint32_t SWAP_COUNT_MAX   = 4;
    static constexpr uint32_t THREAD_COUNT_MAX = 4;

    struct ThreadCommands
    {
        VkCommandPool cmdPool;
        VkArray<VkCommandBuffer, SWAP_COUNT_MAX> cmdBuffers;
    };
    ThreadCommands threadCommands [THREAD_COUNT_MAX] {};

    void Create()
    {
        const auto poolInfo = CmdPoolInfo(g_contextPtr->queueIndex);
        VkCheck(vkCreateCommandPool(g_contextPtr->device, &poolInfo, nullptr, &mainCmdPool));

        FOR_CARRAY(threadCommands, i) {
            auto& cmdPool    = threadCommands[i].cmdPool;
            auto& cmdBuffers = threadCommands[i].cmdBuffers;
            VkCheck(vkCreateCommandPool(g_contextPtr->device, &poolInfo, nullptr, &cmdPool));

            cmdBuffers.count = g_contextPtr->swapImages.count;
            const auto allocInfo = CmdBufferAllocInfo(cmdPool, cmdBuffers.count);
            VkCheck(vkAllocateCommandBuffers(g_contextPtr->device, &allocInfo, cmdBuffers.data));
        }
    }

    void Destroy()
    {
        vkDestroyCommandPool(g_contextPtr->device, mainCmdPool, nullptr);

        FOR_CARRAY(threadCommands, i) {
            auto& cmdPool    = threadCommands[i].cmdPool;
            auto& cmdBuffers = threadCommands[i].cmdBuffers;
            FOR_VK_ARRAY(cmdBuffers, i)
                vkFreeCommandBuffers(g_contextPtr->device, cmdPool, 1, &cmdBuffers[i]);
            cmdBuffers.count = 0;
            vkDestroyCommandPool(g_contextPtr->device, cmdPool, nullptr);
        }
    }
    ~Commands()
    {
        Destroy();
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