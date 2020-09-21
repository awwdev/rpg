//https://github.com/awwdev
#pragma once

#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Meta/Commands.hpp"
#include "GPU/Vulkan/Helper/Utils.hpp"

namespace rpg::gpu::vuk {

struct Image
{
    VkImage         image { VK_NULL_HANDLE };
    VkDeviceMemory  memory;
    VkImageView     view;
    VkImageLayout   layout;

    void Create(
        const VkFormat format,
        const uint32_t width, const uint32_t height,
        const VkSampleCountFlagBits sampleCount,
        const VkImageUsageFlags usage,
        const VkImageAspectFlags aspect,
        const uint32_t layerCount = 1,
        const VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D)
    {
        //? IMAGE
        const VkImageCreateInfo imageInfo {
            .sType                  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .imageType              = VK_IMAGE_TYPE_2D,
            .format                 = format,
            .extent                 = VkExtent3D { width, height, 1 },
            .mipLevels              = 1,
            .arrayLayers            = layerCount,
            .samples                = sampleCount,
            .tiling                 = VK_IMAGE_TILING_OPTIMAL,
            .usage                  = usage,
            .sharingMode            = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount  = 0,
            .pQueueFamilyIndices    = 0,
            .initialLayout          = VK_IMAGE_LAYOUT_UNDEFINED
        };
        VkCheck(vkCreateImage(g_contextPtr->device, &imageInfo, nullptr, &image));

        //? MEMORY
        VkMemoryRequirements memReqs;
        vkGetImageMemoryRequirements(g_contextPtr->device, image, &memReqs);
        const VkMemoryPropertyFlags memProps { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT };
        const auto allocInfo = MemoryAllocInfo(memReqs.size, MemoryType(g_contextPtr->physicalMemProps, memReqs, memProps));
        VkCheck(vkAllocateMemory(g_contextPtr->device, &allocInfo, nullptr, &memory));
        VkCheck(vkBindImageMemory(g_contextPtr->device, image, memory, 0));

        //? VIEW
        const VkImageViewCreateInfo viewInfo {
            .sType              = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext              = nullptr,
            .flags              = 0, 
            .image              = image, 
            .viewType           = viewType, 
            .format             = format,
            .components         = 
            {
                .r = VK_COMPONENT_SWIZZLE_R,
                .g = VK_COMPONENT_SWIZZLE_G,
                .b = VK_COMPONENT_SWIZZLE_B,
                .a = VK_COMPONENT_SWIZZLE_A
            },
            .subresourceRange   = 
            {
                .aspectMask     = aspect,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = layerCount
            }
        };
        VkCheck(vkCreateImageView(g_contextPtr->device, &viewInfo, nullptr, &view));
    }

    void Transition(
        VkCommandPool cmdPool,
        VkImageLayout newLayout,
        VkImageAspectFlags aspect, 
        VkAccessFlags dstAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT | VK_ACCESS_TRANSFER_READ_BIT, 
        VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT)
    {
        layout = newLayout; 
        auto cmdBuffer = BeginCommands_OneTime(g_contextPtr->device, cmdPool);

        const VkImageMemoryBarrier barrier
        {
            .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext               = nullptr,
            .srcAccessMask       = 0,
            .dstAccessMask       = dstAccessMask,
            .oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED,
            .newLayout           = newLayout,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image               = image,
            .subresourceRange    = 
            {
                .aspectMask      = aspect,
                .baseMipLevel    = 0,
                .levelCount      = 1,
                .baseArrayLayer  = 0,
                .layerCount      = 1
            }
        };

        vkCmdPipelineBarrier(
            cmdBuffer,
            dstStageMask, 
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        EndCommands_OneTime(g_contextPtr->device, cmdBuffer, cmdPool, g_contextPtr->queue);
    }

    void Destroy()
    {
        if (image != VK_NULL_HANDLE) {
            vkDestroyImage      (g_contextPtr->device, image, nullptr);
            vkFreeMemory        (g_contextPtr->device, memory, nullptr);
            vkDestroyImageView  (g_contextPtr->device, view, nullptr);
        }
        image = VK_NULL_HANDLE;
    }

    ~Image()
    {
        Destroy();
    }
};

}//ns