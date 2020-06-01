#pragma once
#include "mini/Memory/Allocator.hpp"
#include "mini/Resources/TextureLoader.hpp"
#include "mini/Box/Map.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Debug/Logger.hpp"

namespace mini::hostRes
{
    struct TextureInfo
    {
        chars_t path;
        u32 w, h;
    };

    //! ADD RESOURCES HERE---------------------------------------------------
    //always add both name and map entry
    enum TextureName 
    {
        Texture1, 
        Texture2,
        ENUM_END
    };

    using TextureInfos = box::Map<TextureInfo, TextureName::ENUM_END>;
    using P = TextureInfos::Pair_t;
    const TextureInfos TEXTURE_INFOS //size is used to figure out which memory to claim (texture array)
    {
        P{ TextureName::Texture1, { "res/Textures/Texture1.bmp", 32, 32 } },
        P{ TextureName::Texture2, { "res/Textures/Texture2.bmp", 48, 48 } },
    };
    //! ---------------------------------------------------------------------

    struct Textures
    {
        //used to auto create gpu resources! (vk::Image)
        ITexture* iTextures[TextureName::ENUM_END]; //size agnostic table

        //? ARRAY OF SIZES 
        //! add to the Claim/Load method!
        mem::BlockPtr<box::Array<Texture<  32,   32>, 2>> ptrTextures1 { mem::AutoClaim::Yes };
        mem::BlockPtr<box::Array<Texture<1024, 1024>, 2>> ptrTextures2 { mem::AutoClaim::Yes };

        inline void Load()
        {
            FOR_MAP_BEGIN(TEXTURE_INFOS, i)
                AssignTextureToArray(i, 
                    //! ADD HERE ALL TEXTURE ARRAYS
                    ptrTextures1, 
                    ptrTextures2
                ); 
                iTextures[i]->LoadFrom_BMP(TEXTURE_INFOS.Get(i).path);
            FOR_MAP_END
        }

        template<class... T>
        void AssignTextureToArray(const u32 idx, T&... arrays)
        {
            const auto fn = [&](auto& arr)
            {
                using Texture_T = typename std::decay_t<decltype(arr)>::DATA_T::DATA_T;

                if (iTextures[idx] != nullptr) return;

                const auto& textureInfo = TEXTURE_INFOS.Get(idx);
                if (textureInfo.w <= Texture_T::MAX_WIDTH && textureInfo.h <= Texture_T::MAX_HEIGHT)
                    iTextures[idx] = &arr.Get().AppendReturn(); //"assignment"
            };

            ((fn(arrays)), ...); //unfold allows to have "iteration" over heterogeneous types

            if (iTextures[idx] == nullptr)
                ERR("not texture array found");
        }
    };

    struct HostResources
    {
        Textures textures; //host side ram textures (will be loaded to gpu "vk images")

        HostResources() 
        {
            //currently load is instant inside ctor
            textures.Load();
        }
    };

}//ns