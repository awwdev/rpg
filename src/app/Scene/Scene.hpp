#pragma once
#include "mini/Types.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Box/String.hpp"
#include "mini/Resources/TextureLoader.hpp"

using namespace mini;

namespace app::scene
{
    struct Scene
    {
        box::String<10> name { "Scene1" };
        mem::BlockPtr<res::Texture<32, 32>> pTexture;

        //! move into a resources manager and then use refs in user code
        Scene()
        {
            mem::ClaimBlock(pTexture);
            res::LoadTexture_BMP("res/Textures/Texture.bmp", pTexture.Get());
        }

        inline void Update(const double dt)
        {
            //LOG("update ", name);
        }

    };

}//ns