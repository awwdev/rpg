//https://github.com/awwdev
#pragma once

#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Meta/Commands.hpp"
#include "GPU/Vulkan/_Old/Objects/Buffer.hpp"
#include "Resources/CpuResources.hpp"
#include "Resources/TextureLoader.hpp"

namespace rpg::gpu::vuk {

//? HELPER
inline void TransitionImageLayout(
    VkDevice device, 
    VkCommandPool cmdPool, 
    VkQueue queue,
    VkImageLayout newLayout,
    VkImage image, 
    uint32_t layerCount = 1)
{
    auto cmdBuffer = BeginCommands_OneTime(device, cmdPool);

    //const auto [srcAccess, dstAccess, srcStage, dstStage] = 
    //[&]() -> std::tuple<VkAccessFlags, VkAccessFlags, VkPipelineStageFlags, VkPipelineStageFlags> { 
    //    if (img.layout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    //        return {
    //            0, VK_ACCESS_TRANSFER_WRITE_BIT,
    //            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT
    //        };
    //    } 
    //    if (img.layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL  && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    //        return  {
    //            VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
    //            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
    //        };
    //    } 
    //    return {};
    //}();

    const VkImageMemoryBarrier barrier
    {
        .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext               = nullptr,
        .srcAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT,//srcAccess,
        .dstAccessMask       = VK_ACCESS_SHADER_READ_BIT,//dstAccess,
        .oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED,//img.layout,
        .newLayout           = newLayout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image               = image,
        .subresourceRange    = 
        {
            .aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel    = 0,
            .levelCount      = 1,
            .baseArrayLayer  = 0,
            .layerCount      = layerCount
        }
    };

    //image.layout = newLayout;

    vkCmdPipelineBarrier(
        cmdBuffer,
        //srcStage, dstStage,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    EndCommands_OneTime(device, cmdBuffer, cmdPool, queue);
}

//! CURRENTLY ONLY USABLE AS DEPTH IMAGE
//  NEEDS TO BE MORE GENERIC OR MULTIPLE STRUCTS
//TODO: use ONE struct for different images types, generic

struct DepthImage2
{
    VkImage         image;
    VkDeviceMemory  memory;
    VkImageView     view;
    uint32_t        width, height;
    VkImageLayout   layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    void Create(
        VkCommandPool cmdPool, 
        VkFormat format, 
        VkImageUsageFlags usage,
        VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT)
    {
        width  = g_contextPtr->surfaceCapabilities.currentExtent.width;
        height = g_contextPtr->surfaceCapabilities.currentExtent.height;

        const VkImageCreateInfo imageInfo {
            .sType                  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .imageType              = VK_IMAGE_TYPE_2D,
            .format                 = format,
            .extent                 = VkExtent3D { width, height, 1 },
            .mipLevels              = 1,
            .arrayLayers            = 1,
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
        const auto allocInfo = CreateAllocInfo(memReqs.size, GetMemoryType(g_contextPtr->physicalMemProps, memReqs, memProps));
        VkCheck(vkAllocateMemory(g_contextPtr->device, &allocInfo, nullptr, &memory)); //todo: allocate once for the app and use memory pool
        VkCheck(vkBindImageMemory(g_contextPtr->device, image, memory, 0));

        const VkImageViewCreateInfo viewInfo 
        {
            .sType              = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext              = nullptr,
            .flags              = 0, 
            .image              = image, 
            .viewType           = VK_IMAGE_VIEW_TYPE_2D, 
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
                .aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1
            }
        };

        VkCheck(vkCreateImageView(g_contextPtr->device, &viewInfo, nullptr, &view));

        //? TRANSITION

        auto cmdBuffer = BeginCommands_OneTime(g_contextPtr->device, cmdPool);

        const VkImageMemoryBarrier barrier
        {
            .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext               = nullptr,
            .srcAccessMask       = 0,
            .dstAccessMask       = 
                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
            .oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED,
            //.newLayout           = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .newLayout           = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image               = image,
            .subresourceRange    = 
            {
                .aspectMask      = VK_IMAGE_ASPECT_DEPTH_BIT,
                .baseMipLevel    = 0,
                .levelCount      = 1,
                .baseArrayLayer  = 0,
                .layerCount      = 1
            }
        };

        //image.layout = newLayout;

        vkCmdPipelineBarrier(
            cmdBuffer,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        EndCommands_OneTime(g_contextPtr->device, cmdBuffer, cmdPool, g_contextPtr->queue);
    }

    void Clear()
    {
        if (image != VK_NULL_HANDLE){
            vkDestroyImage      (g_contextPtr->device, image, nullptr);
            vkFreeMemory        (g_contextPtr->device, memory, nullptr);
            vkDestroyImageView  (g_contextPtr->device, view, nullptr);
        }
        image = VK_NULL_HANDLE;
    }

    ~DepthImage2()
    {
        Clear();
    }
};

//! already have ImageArray.hpp
struct DepthImageArray
{
    VkImage         image; //combined
    VkDeviceMemory  memory;
    VkImageView     view;

    uint32_t width, height;
    VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    void Create(
        VkCommandPool cmdPool, 
        VkFormat format, 
        uint32_t layerCount,
        uint32_t pWidth, uint32_t pHeight,
        VkImageUsageFlags usage,
        VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT)
    {
        width  = pWidth; 
        height = pHeight;

        const VkImageCreateInfo imageInfo {
            .sType                  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .imageType              = VK_IMAGE_TYPE_2D,
            .format                 = format,
            .extent                 = VkExtent3D { width, height, 1 },
            .mipLevels              = 1,
            .arrayLayers            = layerCount, //!
            .samples                = sampleCount,
            .tiling                 = VK_IMAGE_TILING_OPTIMAL,
            .usage                  = usage, //VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
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
        const auto allocInfo = CreateAllocInfo(memReqs.size, GetMemoryType(g_contextPtr->physicalMemProps, memReqs, memProps));
        VkCheck(vkAllocateMemory(g_contextPtr->device, &allocInfo, nullptr, &memory)); //todo: allocate once for the app and use memory pool
        VkCheck(vkBindImageMemory(g_contextPtr->device, image, memory, 0));

        const VkImageViewCreateInfo viewInfo 
        {
            .sType              = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext              = nullptr,
            .flags              = 0, 
            .image              = image, 
            .viewType           = VK_IMAGE_VIEW_TYPE_2D_ARRAY, //! 
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
                .aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = layerCount //!
            }
        };
        VkCheck(vkCreateImageView(g_contextPtr->device, &viewInfo, nullptr, &view));

        //? TRANSITION

        auto cmdBuffer = BeginCommands_OneTime(g_contextPtr->device, cmdPool);

        const VkImageMemoryBarrier barrier
        {
            .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext               = nullptr,
            .srcAccessMask       = 0,
            .dstAccessMask       = 
                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
            .oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED,
            //.newLayout           = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
            .newLayout           = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image               = image,
            .subresourceRange    = 
            {
                .aspectMask      = VK_IMAGE_ASPECT_DEPTH_BIT,
                .baseMipLevel    = 0,
                .levelCount      = 1,
                .baseArrayLayer  = 0,
                .layerCount      = 1
            }
        };

        //image.layout = newLayout;

        vkCmdPipelineBarrier(
            cmdBuffer,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        EndCommands_OneTime(g_contextPtr->device, cmdBuffer, cmdPool, g_contextPtr->queue);
    }


    ~DepthImageArray()
    {
        Destroy();
    }

    void Destroy()
    {
        if (image != VK_NULL_HANDLE){
            vkDestroyImage      (g_contextPtr->device, image, nullptr);
            vkFreeMemory        (g_contextPtr->device, memory, nullptr);
            vkDestroyImageView  (g_contextPtr->device, view, nullptr);
        }
        image = VK_NULL_HANDLE;
    }
};

struct MSAAImage2
{
    VkImage         image { VK_NULL_HANDLE };
    VkDeviceMemory  memory;
    VkImageView     view;
    uint32_t        width, height;
    VkImageLayout   layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    void Create(VkCommandPool cmdPool, VkFormat format, VkSampleCountFlagBits sampleCount)
    {
        width  = g_contextPtr->surfaceCapabilities.currentExtent.width;
        height = g_contextPtr->surfaceCapabilities.currentExtent.height;

        const VkImageCreateInfo imageInfo {
            .sType                  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .imageType              = VK_IMAGE_TYPE_2D,
            .format                 = format,
            .extent                 = VkExtent3D { width, height, 1 },
            .mipLevels              = 1,
            .arrayLayers            = 1,
            .samples                = sampleCount,
            .tiling                 = VK_IMAGE_TILING_OPTIMAL,
            .usage                  = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |  VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
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
        const auto allocInfo = CreateAllocInfo(memReqs.size, GetMemoryType(g_contextPtr->physicalMemProps, memReqs, memProps));
        VkCheck(vkAllocateMemory(g_contextPtr->device, &allocInfo, nullptr, &memory)); //todo: allocate once for the app and use memory pool
        VkCheck(vkBindImageMemory(g_contextPtr->device, image, memory, 0));

        const VkImageViewCreateInfo viewInfo 
        {
            .sType              = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext              = nullptr,
            .flags              = 0, 
            .image              = image, 
            .viewType           = VK_IMAGE_VIEW_TYPE_2D, 
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
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1
            }
        };

        VkCheck(vkCreateImageView(g_contextPtr->device, &viewInfo, nullptr, &view));

        //? TRANSITION

        auto cmdBuffer = BeginCommands_OneTime(g_contextPtr->device, cmdPool);

        const VkImageMemoryBarrier barrier
        {
            .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext               = nullptr,
            .srcAccessMask       = 0,
            .dstAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT,
            .oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED,
            .newLayout           = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image               = image,
            .subresourceRange    = 
            {
                .aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel    = 0,
                .levelCount      = 1,
                .baseArrayLayer  = 0,
                .layerCount      = 1
            }
        };

        //image.layout = newLayout;

        vkCmdPipelineBarrier(
            cmdBuffer,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        EndCommands_OneTime(g_contextPtr->device, cmdBuffer, cmdPool, g_contextPtr->queue);
    }


    ~MSAAImage2()
    {
        Clear();
    }

    void Clear()
    {
        if(image != VK_NULL_HANDLE) { 
            vkDestroyImage      (g_contextPtr->device, image, nullptr);
            vkFreeMemory        (g_contextPtr->device, memory, nullptr);
            vkDestroyImageView  (g_contextPtr->device, view, nullptr);
        }
        image = VK_NULL_HANDLE;
    }
};

struct RenderImage2
{
    VkImage         image { VK_NULL_HANDLE };
    VkDeviceMemory  memory;
    VkImageView     view;
    uint32_t        width, height;
    VkImageLayout   layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    void Create(VkCommandPool cmdPool, VkFormat format)
    {
        width  = g_contextPtr->surfaceCapabilities.currentExtent.width;
        height = g_contextPtr->surfaceCapabilities.currentExtent.height;

        const VkImageCreateInfo imageInfo {
            .sType                  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .imageType              = VK_IMAGE_TYPE_2D,
            .format                 = format,
            .extent                 = VkExtent3D { width, height, 1 },
            .mipLevels              = 1,
            .arrayLayers            = 1,
            .samples                = VK_SAMPLE_COUNT_1_BIT,
            .tiling                 = VK_IMAGE_TILING_OPTIMAL,
            .usage                  = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
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
        const auto allocInfo = CreateAllocInfo(memReqs.size, GetMemoryType(g_contextPtr->physicalMemProps, memReqs, memProps));
        VkCheck(vkAllocateMemory(g_contextPtr->device, &allocInfo, nullptr, &memory)); //todo: allocate once for the app and use memory pool
        VkCheck(vkBindImageMemory(g_contextPtr->device, image, memory, 0));

        const VkImageViewCreateInfo viewInfo 
        {
            .sType              = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext              = nullptr,
            .flags              = 0, 
            .image              = image, 
            .viewType           = VK_IMAGE_VIEW_TYPE_2D, 
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
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1
            }
        };

        VkCheck(vkCreateImageView(g_contextPtr->device, &viewInfo, nullptr, &view));
    }

    ~RenderImage2()
    {
        Clear();
    }

    void Clear()
    {
        if(image != VK_NULL_HANDLE) { 
            vkDestroyImage      (g_contextPtr->device, image, nullptr);
            vkFreeMemory        (g_contextPtr->device, memory, nullptr);
            vkDestroyImageView  (g_contextPtr->device, view, nullptr);
        }
        image = VK_NULL_HANDLE;
    }
};

}//ns