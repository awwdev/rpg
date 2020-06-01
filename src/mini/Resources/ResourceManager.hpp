#pragma once
#include "mini/Memory/Allocator.hpp"
#include "mini/Resources/TextureLoader.hpp"
#include "mini/Box/Map.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Debug/Logger.hpp"

namespace mini::res
{
    struct TextureData
    {
        chars_t path;
        u32 x, y;
    };

    //! ADD RESOURCES HERE---------------------------------------------------
    //always add both name and map entry
    enum TextureName 
    {
        Texture1, 
        Texture2,
        ENUM_END
    };

    using TextureMap = box::Map<TextureData, TextureName::ENUM_END>;
    using P = TextureMap::Pair_t;
    const TextureMap TEXTURE_MAP
    {
        P{ TextureName::Texture1, { "res/Textures/Texture1.bmp", 32, 32 } },
        P{ TextureName::Texture2, { "res/Textures/Texture2.bmp", 48, 48 } },
    };
    //! ---------------------------------------------------------------------


    template<class... T>
    void AssignTextureToTextureArray(
        const u32 idx, 
        ITexture* (&textures) [TextureName::ENUM_END], 
        T&... arrays)
    {
        bool assigned = false;

        const auto fn = [&](auto& arr)
        {
            if (assigned) return;

            auto& data = TEXTURE_MAP.Get(idx);
            if (data.x <= std::decay_t<decltype(arr)>::DATA_T::DATA_T::MAX_WIDTH && 
                data.y <= std::decay_t<decltype(arr)>::DATA_T::DATA_T::MAX_HEIGHT) {
                textures[idx] = &arr.Get().AppendReturn();
                assigned = true;
            } 
        };

        ((fn(arrays)), ...);

        if (assigned == false)
            ERR("not texture array found");
    }

    struct Textures
    {
        //used to create gpu resources! (vk::Image)
        ITexture* textures[TextureName::ENUM_END]; //size agnostic table

        //? ARRAY OF SIZES
        mem::BlockPtr<box::Array<Texture<  32,   32>, 2>> ptrTextures1 { mem::AutoClaim::Yes };
        mem::BlockPtr<box::Array<Texture<1024, 1024>, 2>> ptrTextures2 { mem::AutoClaim::Yes };

        inline void Load()
        {
            FOR_MAP_BEGIN(TEXTURE_MAP, i)
                AssignTextureToTextureArray(i, textures, ptrTextures1, ptrTextures2);
                textures[i]->LoadFrom_BMP(TEXTURE_MAP.Get(i).path);
            FOR_MAP_END
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