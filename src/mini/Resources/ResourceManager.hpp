#pragma once
#include "mini/Resources/TextureLoader.hpp"
#include "mini/Memory/Allocator.hpp"

namespace mini::res
{
    //? RESOURCES

    struct Textures
    {
        mem::BlockPtr<Texture<32, 32>> pTexture;

        inline void Load()
        {
            res::LoadTexture_BMP("res/Textures/Texture.bmp", pTexture);
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