#pragma once
#include "mini/Memory/Allocator.hpp"
#include "mini/Resources/TextureLoader.hpp"

namespace mini::res
{
    //? RESOURCES

    struct Textures
    {
        mem::BlockPtr<Texture<32, 32>> pFont;

        inline void Load()
        {
            res::LoadTexture_BMP("res/Textures/Texture.bmp", pFont);
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