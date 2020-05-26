//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Objects/Image.hpp"

#include "mini/Resources/ResourceManager.hpp"

namespace mini::vk
{
    struct Img 
    {

    };


    struct Images
    {
        Image images [res::ENUM_END];

        //? DEFINE IMAGES AND RESOURCES HERE: -------------------------------------
        //Image image_font;

        inline void LoadImages(Context& context, res::ResourceManager& resManager) 
        {
            FOR_CARRAY(images, i)
                images[i].Create(context, *resManager.textures.mapping[i], cmdPool);
            //images[res::Font].Create();
            //image_font.Create(context, resManager.textures.ptrFont.Get(), cmdPool);
        }
        //? -----------------------------------------------------------------------



        VkDevice device;
        VkCommandPool cmdPool;

        inline void Create(Context& context, res::ResourceManager& resManager)
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

            LoadImages(context, resManager);
        }

        ~Images()
        {
            vkDestroyCommandPool(device, cmdPool, nullptr);
        }
    };

}//ns