//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Ctors.hpp"

namespace mini::vk
{
    struct Commands
    {
        VkCommandPool cmdPool;
        box::POD_Array<VkCommandBuffer, 4> cmdBuffers { 0 };

        inline void Create()
        {
            //? CMD POOL
            const VkCommandPoolCreateInfo poolInfo
            {
                .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                .pNext = nullptr,
                .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                .queueFamilyIndex = context.queueIndex
            };
            VK_CHECK(vkCreateCommandPool(context.device, &poolInfo, nullptr, &cmdPool));

            //? CMD BUFFERS

            cmdBuffers.count = context.swapImages.count;
            const VkCommandBufferAllocateInfo allocInfo
            {
             .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
             .pNext              = nullptr,
             .commandPool        = cmdPool,
             .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
             .commandBufferCount = cmdBuffers.count
            };
            VK_CHECK(vkAllocateCommandBuffers(context.device, &allocInfo, cmdBuffers.data));
        }

        ~Commands()
        {
            vkFreeCommandBuffers(context.device, cmdPool, cmdBuffers.count, cmdBuffers.data);
            vkDestroyCommandPool(context.device, cmdPool, nullptr);
        }
    };

} //ns