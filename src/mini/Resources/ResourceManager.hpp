#pragma once
#include "mini/Memory/Allocator.hpp"
#include "mini/Resources/TextureLoader.hpp"
#include "mini/Box/String.hpp"

#include "mini/Debug/Logger.hpp"

#include <unordered_map>

namespace mini::res
{
    struct TextureData
    {
        chars_t path;
        u32 x, y;
    };

    //! ADD RESOURCES HERE---------------------------------------------------
    enum TextureName 
    {
        Texture1, 
        Texture2,
        ENUM_END
    };

    const std::unordered_map<TextureName, TextureData> mapping 
    {
        { TextureName::Texture1, { "res/Textures/Texture1.bmp",  32, 32 } },
        { TextureName::Texture2,  { "res/Textures/Texture2.bmp", 48, 48 } },
    };
    //! ---------------------------------------------------------------------



    struct Textures
    {
        ITexture* textures[TextureName::ENUM_END]; //size agnostic table

        //? ARRAY OF SIZES
        using TextureArray1 = mem::BlockPtr<box::Array<Texture<  32,   32>, 2>>;
        using TextureArray2 = mem::BlockPtr<box::Array<Texture<1024, 1024>, 2>>;

        TextureArray1 ptrTextures1;
        TextureArray2 ptrTextures2;


        inline void Load()
        {
            mem::ClaimBlock(ptrTextures1);
            mem::ClaimBlock(ptrTextures2);

            for(const auto& [name, data] : mapping)
            {
                //find appropriate texture array
                if (data.x <= TextureArray1::DATA_T::DATA_T::MAX_WIDTH && data.y <= TextureArray1::DATA_T::DATA_T::MAX_HEIGHT) {
                    textures[name] = &ptrTextures1.Get().AppendReturn();
                } else
                if (data.x <= TextureArray2::DATA_T::DATA_T::MAX_WIDTH && data.y <= TextureArray2::DATA_T::DATA_T::MAX_HEIGHT) {
                    textures[name] = &ptrTextures2.Get().AppendReturn();
                }
                else {
                    ERR("no texture array found!");
                }

                textures[name]->LoadFrom_BMP(data.path);
            }
        }
    };


    //? RESOURCE MANAGER

    struct ResourceManager
    {
        Textures textures;

        ResourceManager() //currently instant inside ctor
        {
            textures.Load();
        }
    };

}//ns