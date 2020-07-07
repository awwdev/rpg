//https://github.com/awwdev
#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Commands.hpp"
#include "mini/Vulkan/Objects/Buffer.hpp"
#include "mini/Vulkan/Objects/Image.hpp"
#include "mini/Resources/HostResources.hpp"
#include "mini/Resources/TextureLoader.hpp"

namespace mini::vk
{
    struct ImageArray
    {
        //data
        VkImage         image;
        VkDeviceMemory  memory;
        VkImageView     view;
        uint32_t        width, height;
        VkImageLayout   layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        template<u32 N, u32 W, u32 H, u32 C> //assuming rgba only for now
        void Create(const hostRes::TextureArray<N, W, H, C>& textureArray, VkCommandPool cmdPool)
        {
            width  = textureArray.WIDTH;
            height = textureArray.HEIGHT;
            constexpr auto FORMAT = VK_FORMAT_R8_SRGB; //!

            //? IMAGE
            const VkImageCreateInfo imageInfo {
                .sType                  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .pNext                  = nullptr,
                .flags                  = 0,
                .imageType              = VK_IMAGE_TYPE_2D,
                .format                 = FORMAT,
                .extent                 = VkExtent3D { width, height, 1 },
                .mipLevels              = 1,
                .arrayLayers            = textureArray.COUNT, //!
                .samples                = VK_SAMPLE_COUNT_1_BIT,
                .tiling                 = VK_IMAGE_TILING_OPTIMAL,
                .usage                  = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                .sharingMode            = VK_SHARING_MODE_EXCLUSIVE,
                .queueFamilyIndexCount  = 0,
                .pQueueFamilyIndices    = 0,
                .initialLayout          = VK_IMAGE_LAYOUT_UNDEFINED
            };
            VK_CHECK(vkCreateImage(g_contextPtr->device, &imageInfo, nullptr, &image));

            //? MEMORY
            VkMemoryRequirements memReqs;
            vkGetImageMemoryRequirements(g_contextPtr->device, image, &memReqs);

            const VkMemoryPropertyFlags memProps { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT };
            const auto allocInfo = CreateAllocInfo(memReqs.size, GetMemoryType(g_contextPtr->physicalMemProps, memReqs, memProps));
            VK_CHECK(vkAllocateMemory(g_contextPtr->device, &allocInfo, nullptr, &memory)); //todo: allocate once for the app and use memory pool
            VK_CHECK(vkBindImageMemory(g_contextPtr->device, image, memory, 0));

            //? VIEW
            const VkImageViewCreateInfo viewInfo {
                .sType              = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext              = nullptr,
                .flags              = 0, 
                .image              = image, 
                .viewType           = VK_IMAGE_VIEW_TYPE_2D_ARRAY, //!
                .format             = FORMAT,
                .components         = 
                {
                    .r = VK_COMPONENT_SWIZZLE_B,//VK_COMPONENT_SWIZZLE_R,
                    .g = VK_COMPONENT_SWIZZLE_G,
                    .b = VK_COMPONENT_SWIZZLE_R,//VK_COMPONENT_SWIZZLE_B, 
                    .a = VK_COMPONENT_SWIZZLE_A
                },
                .subresourceRange   = 
                {
                    .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel   = 0, //isnt this 1 ???
                    .levelCount     = 1,
                    .baseArrayLayer = 0,
                    .layerCount     = textureArray.COUNT
                }
            };
            VK_CHECK(vkCreateImageView(g_contextPtr->device, &viewInfo, nullptr, &view));

            //? LOAD
            TransitionImageLayout(g_contextPtr->device, cmdPool, g_contextPtr->queue, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, image, textureArray.COUNT);

            //? TMP BUFFER
            Buffer buffer;
            buffer.Create(
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                textureArray.TOTAL_SIZE, //!
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            );
            buffer.Store(textureArray.data, textureArray.TOTAL_SIZE);
            
            //? COPY FROM BUFFER
            auto cmdBuffer = BeginCommands_OneTime(g_contextPtr->device, cmdPool);

            VkBufferImageCopy regions [N];
            FOR_CARRAY(regions, i) {
                regions[i] = {
                    .bufferOffset       = i * textureArray.TEX_SIZE,
                    .bufferRowLength    = 0,
                    .bufferImageHeight  = 0,
                    .imageSubresource   = 
                    {
                        .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                        .mipLevel       = 0,
                        .baseArrayLayer = (uint32_t)i,
                        .layerCount     = 1
                    },
                    .imageOffset        = { 0, 0, 0 },
                    .imageExtent        = { width, height, 1 }
                };
            }

            vkCmdCopyBufferToImage(cmdBuffer, buffer.buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, textureArray.COUNT, regions);
            EndCommands_OneTime(g_contextPtr->device, cmdBuffer, cmdPool, g_contextPtr->queue);
            TransitionImageLayout(g_contextPtr->device, cmdPool, g_contextPtr->queue, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, image, textureArray.COUNT);
        }

        ~ImageArray()
        {
            vkDestroyImage      (g_contextPtr->device, image, nullptr);
            vkFreeMemory        (g_contextPtr->device, memory, nullptr);
            vkDestroyImageView  (g_contextPtr->device, view, nullptr);
        }
    };

}//ns