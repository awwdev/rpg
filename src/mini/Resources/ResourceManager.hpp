#pragma once
#include "mini/Memory/Allocator.hpp"
#include "mini/Resources/TextureLoader.hpp"

#include "mini/Debug/Logger.hpp"

namespace mini::res
{
    //? RESOURCES
    //! this is automatically used by vk::Images !!!
    enum TextureName { Font, ENUM_END };

    struct Textures
    {
        ITexture* mapping [ENUM_END];

        mem::BlockPtr<Texture<32, 32>> ptrFont;

        inline void Load()
        {
            mem::ClaimBlock(ptrFont);
            res::LoadTexture_BMP("res/Textures/Texture.bmp", *ptrFont);
            mapping[Font] = &*ptrFont;
        }
    };


    //? RESOURCE MANAGER

    struct ResourceManager
    {
        Textures textures;

        ResourceManager()
        {
            textures.Load();
        }
    };

}//ns