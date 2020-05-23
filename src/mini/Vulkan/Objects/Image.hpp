//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Resources.hpp"
#include "mini/Vulkan/Commands.hpp"
#include "mini/Vulkan/Objects/Buffer.hpp"

#include "mini/Resources/TextureLoader.hpp"

namespace mini::vk
{
    //? HELPER

    inline void TransitionImageLayout(
        VkDevice device, 
        VkCommandPool cmdPool, 
        VkQueue queue,
        VkImageLayout newLayout,
        VkImage image)
    {
        auto cmdBuffer = BeginSingleTimeCommands(device, cmdPool);

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

        const VkImageMemoryBarrier barrier{
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
                .layerCount      = 1
            }
        };

        //img.layout = newLayout;

        vkCmdPipelineBarrier(
            cmdBuffer,
            //srcStage, dstStage,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        EndSingleTimeCommands(device, cmdBuffer, cmdPool, queue);
    }


    //? IMG OBJECT

    struct Image
    {
        //refs
        VkDevice device;
        VkPhysicalDeviceMemoryProperties physicalMemProps; //meh
        VkCommandPool cmdPool;
        VkQueue queue;

        //data
        VkImage image;
        VkDeviceMemory memory;
        VkImageView view;
        uint32_t width, height;


        inline void Create(
            Context& pContext, 
            Commands& commands, 
            const uint32_t pWidth, const uint32_t pHeight)
        {
            device  = pContext.device;
            queue   = pContext.queue;
            cmdPool = commands.cmdPool;
            physicalMemProps = pContext.physicalMemProps;

            width   = pWidth;
            height  = pHeight;

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
                .usage                  = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
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
            const auto allocInfo = CreateAllocInfo(memReqs.size, GetMemoryType(physicalMemProps, memReqs, memProps));
            VK_CHECK(vkAllocateMemory(device, &allocInfo, nullptr, &memory)); //todo: allocate once for the app and use memory pool
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

            VK_CHECK(vkCreateImageView(device, &viewInfo, nullptr, &view));
        }


        template<u32 WIDTH, u32 HEIGHT>
        inline void Load(mini::res::Texture<WIDTH, HEIGHT>& texture)
        {
            //? TMP BUFFER

            Buffer buffer;
            buffer.Create(
                device,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                texture.SIZE,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                physicalMemProps
            );
            buffer.Store(texture.texPtr, texture.SIZE);
            
            TransitionImageLayout(device, cmdPool, queue, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, image);
            
            //? COPY FROM BUFFER

            auto cmdBuffer = BeginSingleTimeCommands(device, cmdPool);

            const VkBufferImageCopy region
            {
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
                .imageExtent        = { WIDTH, HEIGHT, 1 }
            };

            vkCmdCopyBufferToImage(cmdBuffer, buffer.buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
            EndSingleTimeCommands(device, cmdBuffer, cmdPool, queue);

            TransitionImageLayout(device, cmdPool, queue, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, image);
        }


        ~Image()
        {
            vkDestroyImage(device, image, nullptr);
            vkFreeMemory(device, memory, nullptr);
            vkDestroyImageView(device, view, nullptr);
        }
    };


}//ns