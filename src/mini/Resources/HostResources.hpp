#pragma once
#include "mini/Memory/Allocator.hpp"
#include "mini/Resources/TextureLoader.hpp"
#include "mini/Resources/Font.hpp"
#include "mini/Box/Map.hpp"
#include "mini/Box/Array.hpp"
#include "mini/Debug/Logger.hpp"

#include <fstream>
#include <filesystem>

namespace mini::hostRes
{
    struct ITextureArray
    {
        const u32 WIDTH;
        const u32 HEIGHT;
        const u32 CHANNELS;
        const u32 COUNT;
        const u32 TEX_SIZE;
        const u32 TOTAL_SIZE;

        char* const dataPtr;
    };

    template<u32 N, u32 W, u32 H, u8 C = 4>
    struct TextureArray : ITextureArray
    {
        constexpr static auto WIDTH      = W;
        constexpr static auto HEIGHT     = H;
        constexpr static auto CHANNELS   = C;
        constexpr static auto COUNT      = N;
        constexpr static auto TEX_SIZE   = W*H*C;
        constexpr static auto TOTAL_SIZE = TEX_SIZE * N;

        char data [COUNT][TEX_SIZE];
        u32 count = 0;


        TextureArray() : ITextureArray
        {
            .WIDTH      = WIDTH,
            .HEIGHT     = HEIGHT,
            .CHANNELS   = CHANNELS,
            .COUNT      = COUNT,
            .TEX_SIZE   = TEX_SIZE,
            .TOTAL_SIZE = TOTAL_SIZE,
            .dataPtr    = &(data[0][0])
        }
        {;}

        void LoadSingle(const std::filesystem::path& path)
        {
            std::ifstream file(path, std::ios::binary);
            file.read(&(data[count][0]), TEX_SIZE);
            ++count;
        }

        void LoadArray(chars_t dir)
        {
            for(auto& p: std::filesystem::directory_iterator(dir)) { 
                LoadSingle(p.path());
            }
        }

    };
















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
        Font,
        ENUM_END
    };

    using TextureInfos = box::Map<TextureInfo, TextureName::ENUM_END>;
    using P = TextureInfos::Pair_t;
    const TextureInfos TEXTURE_INFOS //size is used to figure out which memory to claim (texture array)
    { //order does not matter
        P{ TextureName::Texture1,   { "res/Textures/Texture1.bmp",  32, 32 } },
        P{ TextureName::Texture2,   { "res/Textures/Texture2.bmp",  48, 48 } },
        P{ TextureName::Font,       { "res/Textures/Font.bmp",      256, 128 } },
    };
    //! ---------------------------------------------------------------------

    struct Textures
    {
        //used to auto create gpu resources! (vk::Image)
        ITexture* iTextures[TextureName::ENUM_END] { nullptr }; //size agnostic table

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
                iTextures[i]->LoadFrom_BMP(TEXTURE_INFOS.GetValue(i).path);
            FOR_MAP_END
        }

        template<class... T>
        void AssignTextureToArray(const u32 idx, T&... arrays)
        {
            u32 arrayIdx = 0;

            const auto fn = [&](auto& arr)
            {
                using Texture_T = typename std::decay_t<decltype(arr)>::DATA_T::DATA_T;

                if (iTextures[idx] != nullptr) return;

                const auto& textureInfo = TEXTURE_INFOS.GetValue(idx);
                if (textureInfo.w <= Texture_T::MAX_WIDTH && textureInfo.h <= Texture_T::MAX_HEIGHT) 
                {
                    iTextures[idx] = &arr.Get().AppendReturn(); //"assignment"
                    LOG("assign texture to ", arrayIdx);
                }
                ++arrayIdx;    
            };

            ((fn(arrays)), ...); //unfold allows to have "iteration" over heterogeneous types

            if (iTextures[idx] == nullptr)
                ERR("not texture array found");
        }
    };

    struct HostResources
    {
        Textures textures; //host side ram textures (will be loaded to gpu "vk images")
        res::Fonts fonts;

        TextureArray<95, 16, 16, 1> textureArray;

        HostResources() 
        {
            //currently load is instant inside ctor
            textures.Load();
            textureArray.LoadArray("res/TextureArray"); //RLE would be nice but conversion tools are work
        }
    };

}//ns