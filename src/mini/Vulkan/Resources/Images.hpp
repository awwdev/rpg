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
        Image image_font;

        //from ram to vram
        inline void Load(Context& context, res::ResourceManager& resManager, VkCommandPool cmdPool)
        {
            image_font.Create(context, 32, 32);
            image_font.Load(resManager.textures.pTexture.Get(), cmdPool);
        }
    };

}//ns