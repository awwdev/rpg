//https://github.com/awwdev

#pragma once
#include "mini/Debug/Assert.hpp"
#include "mini/Memory/Allocator.hpp"
#include "mini/Vulkan/Core.hpp"

#include <fstream>

//used to create default structs with common params needed
//struct can be customized after return when needed

namespace mini::vk
{
    //? SHADER

    inline VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo 
    (const VkShaderStageFlagBits stage)
    {
        return {
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext  = nullptr,
            .flags  = 0,
            .stage  = stage,
            .module = nullptr,
            .pName  = "main",
            .pSpecializationInfo = nullptr 
        };
    }

    inline VkShaderModule CreateShaderModule(VkDevice device, chars_t path)
    {
        std::ifstream file(path, std::ios::ate | std::ios::binary);
        mini::Assert(file.is_open(), "cannot open shader file");

        const uint32_t size = (uint32_t)file.tellg();
        auto ptrBuffer = mini::mem::ClaimBlock<char[10000]>(); //! capacity
        //char buffer[10000]; //!be aware of capacity, maybe use allocator to not exhaust stack
        file.seekg(std::ios::beg);
        file.read(*ptrBuffer, size);

        const VkShaderModuleCreateInfo moduleInfo {
            .sType      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext      = nullptr,
            .flags      = 0,
            .codeSize   = size,
            .pCode      = reinterpret_cast<const uint32_t*>(*ptrBuffer)
        };

        VkShaderModule mod;
        VK_CHECK(vkCreateShaderModule(device, &moduleInfo, nullptr, &mod));
        return mod;
    }

    //? PRESENTING

    inline VkPresentInfoKHR PresentInfo(VkSemaphore& rendered, VkSwapchainKHR& swapchain, uint32_t& imgIndex)
    {
        return {
            .sType                  = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext                  = nullptr,
            .waitSemaphoreCount     = 1,
            .pWaitSemaphores        = &rendered,
            .swapchainCount         = 1,
            .pSwapchains            = &swapchain,
            .pImageIndices          = &imgIndex,
            .pResults               = nullptr
        };
    }


    inline VkSubmitInfo SubmitInfo(VkSemaphore& aquired, VkSemaphore& rendered, VkCommandBuffer& cmdBuffer)
    {
        const static VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        return {
            .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext                  = nullptr,
            .waitSemaphoreCount     = 1,
            .pWaitSemaphores        = &aquired,
            .pWaitDstStageMask      = &waitStages,
            .commandBufferCount     = 1,
            .pCommandBuffers        = &cmdBuffer,
            .signalSemaphoreCount   = 1,
            .pSignalSemaphores      = &rendered,
        };
    }

    //? COMMANDS

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


    inline void EndCommands_OneTime(
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


}//ns