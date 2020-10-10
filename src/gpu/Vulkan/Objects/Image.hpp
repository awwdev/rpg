//https://github.com/awwdev
#pragma once

#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/Meta/Commands.hpp"
#include "gpu/Vulkan/Objects/Buffer.hpp"
#include "gpu/Vulkan/Helper/Utils.hpp"

#include "com/box/Array.hpp"

namespace rpg::gpu::vuk {

constexpr VkComponentMapping IMAGE_COMPONENT_MAPPING_DEFAULT = 
{ 
    .r = VK_COMPONENT_SWIZZLE_R,
    .g = VK_COMPONENT_SWIZZLE_G,
    .b = VK_COMPONENT_SWIZZLE_B,
    .a = VK_COMPONENT_SWIZZLE_A
};

struct Image
{
    VkImage         image { VK_NULL_HANDLE };
    VkDeviceMemory  memory;
    VkImageView     view;
    VkImageLayout   currentLayout { VK_IMAGE_LAYOUT_UNDEFINED }; //don't change from outside

    static constexpr auto LAYER_COUNT_MAX = 300;
    uint32_t layerCount;
    uint32_t width, height;
    VkImageAspectFlags aspect;

    void Create(
        const VkFormat format,
        const uint32_t pWidth, const uint32_t pHeight,
        const VkSampleCountFlagBits sampleCount,
        const VkImageUsageFlags usage,
        const VkImageAspectFlags pAspect,
        const uint32_t pLayerCount = 1,
        const VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D,
        const VkComponentMapping& componentMapping = IMAGE_COMPONENT_MAPPING_DEFAULT)
    {
        layerCount = pLayerCount;
        dbg::Assert(layerCount < LAYER_COUNT_MAX, "layer count too high");
        width  = pWidth;
        height = pHeight;
        aspect = pAspect;

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
            .initialLayout          = currentLayout
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
        const VkImageViewCreateInfo viewInfo 
        {
            .sType              = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext              = nullptr,
            .flags              = 0, 
            .image              = image, 
            .viewType           = viewType, 
            .format             = format,
            .components         = componentMapping,
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
        VkAccessFlags srcAccessMask,
        VkAccessFlags dstAccessMask, 
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask)
    {
        auto cmdBuffer = BeginCommands_OneTime(g_contextPtr->device, cmdPool);

        const VkImageMemoryBarrier barrier
        {
            .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext               = nullptr,
            .srcAccessMask       = srcAccessMask,
            .dstAccessMask       = dstAccessMask,
            .oldLayout           = currentLayout,
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
                .layerCount      = layerCount
            }
        };

        currentLayout = newLayout;

        vkCmdPipelineBarrier(
            cmdBuffer,
            srcStageMask, 
            dstStageMask,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        EndCommands_OneTime(g_contextPtr->device, cmdBuffer, cmdPool, g_contextPtr->queue);
    }

    void Store(VkCommandPool cmdPool, void const* data, const uint32_t size, const uint32_t singleTextureSize)
    {
        //prepare for storing 
        Transition(
            cmdPool, 
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
            0, VK_ACCESS_TRANSFER_WRITE_BIT,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT
        );

        //copy data into buffer
        Buffer tmpBuffer;
        tmpBuffer.Create(
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
            size, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );
        tmpBuffer.Map();
        tmpBuffer.Store(data, size);

        com::Array<VkBufferImageCopy, LAYER_COUNT_MAX> imageCopyInfos;
        for(uint32_t layerIdx = 0; layerIdx < layerCount; ++layerIdx)
        {
            imageCopyInfos.AppendElement(VkBufferImageCopy {
                .bufferOffset       = layerIdx * singleTextureSize,
                .bufferRowLength    = 0,
                .bufferImageHeight  = 0,
                .imageSubresource   = 
                {
                    .aspectMask     = aspect,
                    .mipLevel       = 0,
                    .baseArrayLayer = layerIdx,
                    .layerCount     = 1
                },
                .imageOffset        = { 0, 0, 0 },
                .imageExtent        = { width, height, 1 }
            });
        }

        //send to queue
        auto cmdBuffer = BeginCommands_OneTime(g_contextPtr->device, cmdPool);
        vkCmdCopyBufferToImage(
            cmdBuffer, 
            tmpBuffer.buffer, image, 
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
            imageCopyInfos.Count(), imageCopyInfos.Data()
        );
        EndCommands_OneTime(g_contextPtr->device, cmdBuffer, cmdPool, g_contextPtr->queue);
    }

    void Bake(VkCommandPool cmdPool)
    {
        Transition(
            cmdPool, 
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
            VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
        );
    }

    void Destroy()
    {
        if (image != VK_NULL_HANDLE) { //! this should not be necessary
            vkDestroyImage      (g_contextPtr->device, image, nullptr);
            vkFreeMemory        (g_contextPtr->device, memory, nullptr);
            vkDestroyImageView  (g_contextPtr->device, view, nullptr);
            currentLayout = VK_IMAGE_LAYOUT_UNDEFINED; //! should not be necessary
            layerCount = 0;
            width  = 0;
            height = 0;
        }
        image = VK_NULL_HANDLE;
    }
    ~Image()
    {
        Destroy();
    }
    
};

}//ns