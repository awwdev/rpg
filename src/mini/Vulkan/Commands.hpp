//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"


namespace mini::vk
{
    //? HELPER

    inline VkCommandBufferBeginInfo CreateCmdBeginInfo(const VkCommandBufferUsageFlags flags = 0)
    {
        return {
            .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext            = nullptr,
            .flags            = flags,
            .pInheritanceInfo = nullptr
        };
    }


    inline VkCommandBuffer BeginSingleTimeCommands(VkDevice device, VkCommandPool cmdPool)
    {
        VkCommandBuffer commandBuffer;

        const VkCommandBufferAllocateInfo allocInfo 
        {
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext              = nullptr,
            .commandPool        = cmdPool,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1
        };
        VK_CHECK(vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer));

        const auto beginInfo = CreateCmdBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));
        
        return commandBuffer;
    }


    inline void EndSingleTimeCommands(
        VkDevice device, 
        VkCommandBuffer cmdBuffer, 
        VkCommandPool cmdPool,
        VkQueue queue)
    {
        VK_CHECK(vkEndCommandBuffer(cmdBuffer));

        const VkSubmitInfo submitInfo = {
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

        VK_CHECK(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
        VK_CHECK(vkQueueWaitIdle(queue));
        vkFreeCommandBuffers(device, cmdPool, 1, &cmdBuffer);
    }
    

    //? DEDICATED STRUCTS

    struct Commands
    {
        VkDevice device;

        VkCommandPool cmdPool;
        VkArray<VkCommandBuffer, 4> cmdBuffers { {}, 0 }; //capacity based


        inline void Create(Context& context)
        {
            device = context.device;

            //? CMD POOL

            const VkCommandPoolCreateInfo poolInfo{
                .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                .pNext = nullptr,
                .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                .queueFamilyIndex = context.queueIndex
            };
            VK_CHECK(vkCreateCommandPool(device, &poolInfo, nullptr, &cmdPool));

            //? CMD BUFFERS

            cmdBuffers.count = context.swapImages.count;
            const VkCommandBufferAllocateInfo allocInfo{
             .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
             .pNext              = nullptr,
             .commandPool        = cmdPool,
             .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
             .commandBufferCount = cmdBuffers.count
            };
            VK_CHECK(vkAllocateCommandBuffers(device, &allocInfo, cmdBuffers.data));
        }

        ~Commands()
        {
            vkFreeCommandBuffers(device, cmdPool, cmdBuffers.count, cmdBuffers.data);
            vkDestroyCommandPool(device, cmdPool, nullptr);
        }
    };

} //ns