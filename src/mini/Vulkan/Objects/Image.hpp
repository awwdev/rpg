//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Resources.hpp"

namespace mini::vk
{
    struct Buffer
    {
        VkDevice device;

        VkImage     image;
        VkImageView view;

        inline void Create(Context& context, const uint32_t width, const uint32_t height)
        {
            device = context.device;

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

        }

        ~Buffer()
        {
            vkDestroyImage(device, image, nullptr);
            vkDestroyImageView(device, view, nullptr);
        }
    };

}//ns