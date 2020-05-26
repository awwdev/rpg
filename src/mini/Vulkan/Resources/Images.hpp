//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Objects/Image.hpp"

#include "mini/Resources/ResourceManager.hpp"

namespace mini::vk
{
    struct Images
    {
        VkDevice device;
        VkCommandPool cmdPool;

        Image image_font;


        inline void Load(Context& context, res::ResourceManager& resManager)
        {
            device = context.device;

            const VkCommandPoolCreateInfo poolInfo
            {
                .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                .pNext = nullptr,
                .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                .queueFamilyIndex = context.queueIndex
            };
            VK_CHECK(vkCreateCommandPool(device, &poolInfo, nullptr, &cmdPool));


            image_font.Create(context, resManager.textures.pFont->WIDTH, resManager.textures.pFont->HEIGHT);
            image_font.Load(resManager.textures.pFont.Get(), cmdPool);
        }

        ~Images()
        {
            vkDestroyCommandPool(device, cmdPool, nullptr);
        }
    };

}//ns