//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Resources.hpp"

namespace mini::vk
{
    inline VkMemoryAllocateInfo CreateAllocInfo(
        const VkDeviceSize& size,
        const uint32_t memTypeIndex)
    {
        return {
            .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext           = nullptr,
            .allocationSize  = size,
            .memoryTypeIndex = memTypeIndex
        };
    }

    inline uint32_t GetMemoryType(
        const VkPhysicalDeviceMemoryProperties& physicalMemProps,
        const VkMemoryRequirements& memReqs,
        const VkMemoryPropertyFlags neededMemProps) 
    {
        for (uint32_t i = 0; i < physicalMemProps.memoryTypeCount; ++i) {
            if (memReqs.memoryTypeBits & (1 << i) &&
                (physicalMemProps.memoryTypes[i].propertyFlags & neededMemProps) == neededMemProps) 
                return i;
        }
        ERR("no suitable memory type found!");
        return {};
    }

    inline VkCommandBufferBeginInfo bufferBeginInfo(const VkCommandBufferUsageFlags flags = 0)
    {
        return {
            .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext            = nullptr,
            .flags            = flags,
            .pInheritanceInfo = nullptr
        };
    }

    inline VkCommandBufferAllocateInfo cmdBufferAllocateInfo(
        VkCommandPool pool,
        const uint32_t cmdBufferCount)
    {
        return {
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext              = nullptr,
            .commandPool        = pool,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = cmdBufferCount
        };
    }

    inline VkCommandBuffer beginSingleTimeCommands(VkDevice device, VkCommandPool cmdPool)
    {
        const auto allocInfo = cmdBufferAllocateInfo(cmdPool, 1);
        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);
        const auto beginInfo = bufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        vkBeginCommandBuffer(commandBuffer, &beginInfo);
        return commandBuffer;
    }

    inline void endSingleTimeCommands(
        VkDevice device, 
        VkCommandBuffer cmdBuffer, 
        VkCommandPool cmdPool,
        VkQueue queue)
    {
        vkEndCommandBuffer(cmdBuffer);

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





    struct Image
    {
        VkDevice device;

        VkImage image;
        VkDeviceMemory memory;
        VkImageView view;
        uint32_t width, height;


        inline void Create(Context& context, const uint32_t pWidth, const uint32_t pHeight)
        {
            device = context.device;
            width  = pWidth;
            height = pHeight;

            //? IMAGE

            const VkImageCreateInfo imageInfo {
                .sType                  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .pNext                  = nullptr,
                .flags                  = 0,
                .imageType              = VK_IMAGE_TYPE_2D,
                .format                 = VK_FORMAT_R8G8B8A8_SRGB,
                .extent                 = VkExtent3D { width, height, 1 },
                .mipLevels              = 1,
                .arrayLayers            = 1,
                .samples                = VK_SAMPLE_COUNT_1_BIT,
                .tiling                 = VK_IMAGE_TILING_OPTIMAL,
                .usage                  = VK_IMAGE_USAGE_SAMPLED_BIT, //VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                .sharingMode            = VK_SHARING_MODE_EXCLUSIVE,
                .queueFamilyIndexCount  = 0,
                .pQueueFamilyIndices    = 0,
                .initialLayout          = VK_IMAGE_LAYOUT_UNDEFINED
            };

            VK_CHECK(vkCreateImage(device, &imageInfo, nullptr, &image));

            //? MEMORY

            VkMemoryRequirements memReqs;
            vkGetImageMemoryRequirements(device, image, &memReqs);

            const VkMemoryPropertyFlags memProps { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT };
            const auto allocInfo = CreateAllocInfo(memReqs.size, GetMemoryType(context.physicalMemProps, memReqs, memProps));
            VK_CHECK(vkAllocateMemory(device, &allocInfo, nullptr, &memory));
            VK_CHECK(vkBindImageMemory(device, image, memory, 0));

            //? VIEW

            const VkImageViewCreateInfo viewInfo {
                .sType              = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext              = nullptr,
                .flags              = 0, 
                .image              = image, 
                .viewType           = VK_IMAGE_VIEW_TYPE_2D, 
                .format             = VK_FORMAT_R8G8B8A8_SRGB,
                .components         = 
                {
                    .r = VK_COMPONENT_SWIZZLE_R,
                    .g = VK_COMPONENT_SWIZZLE_G,
                    .b = VK_COMPONENT_SWIZZLE_B,
                    .a = VK_COMPONENT_SWIZZLE_A
                },
                .subresourceRange   = 
                {
                    .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel   = 0, //isnt this 1 ???
                    .levelCount     = 1,
                    .baseArrayLayer = 0,
                    .layerCount     = 1
                }
            };

            //VK_CHECK(vkCreateImageView(device, &viewInfo, nullptr, &view));

        }


        inline void CopyBufferToImage(VkCommandPool& cmdPool, VkQueue& queue, VkBuffer& buffer)
        {
            auto cmdBuffer = beginSingleTimeCommands(device, cmdPool);

            const VkBufferImageCopy region {
                .bufferOffset       = 0,
                .bufferRowLength    = 0,
                .bufferImageHeight  = 0,
                .imageSubresource   = 
                {
                    .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                    .mipLevel       = 0,
                    .baseArrayLayer = 0,
                    .layerCount     = 1
                },
                .imageOffset        = { 0, 0, 0 },
                .imageExtent        = { width, height, 1 }
            };

            vkCmdCopyBufferToImage(cmdBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
            endSingleTimeCommands(device, cmdBuffer, cmdPool, queue);
        }


        ~Image()
        {
            vkDestroyImage(device, image, nullptr);
            vkFreeMemory(device, memory, nullptr);
            vkDestroyImageView(device, view, nullptr);
        }
    };


}//ns