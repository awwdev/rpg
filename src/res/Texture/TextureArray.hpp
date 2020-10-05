//https://github.com/awwdev
#pragma once

#include "com/Types.hpp"
#include <fstream>
#include <filesystem>

namespace rpg::res {
    
template
<
uint32_t WIDTH_T, 
uint32_t HEIGHT_T, 
uint32_t COUNT_T = 1, 
uint32_t CHANNELS_T = 4
>
struct TextureArray
{
    //TODO: W, H and C as max, and then per texture individual sizes?

    static constexpr auto WIDTH    = WIDTH_T;
    static constexpr auto HEIGHT   = HEIGHT_T;
    static constexpr auto COUNT    = COUNT_T;
    static constexpr auto CHANNELS = CHANNELS_T;

    static constexpr auto SINGLE_TEXTURE_BYTES = WIDTH * HEIGHT * CHANNELS_T * 1; //0-255 range
    static constexpr auto ALL_TEXTURES_BYTES   = SINGLE_TEXTURE_BYTES * COUNT;

    char  data [COUNT][SINGLE_TEXTURE_BYTES];
    idx_t count = 0;

    void LoadFile_RawFormat(const std::filesystem::path& path)
    {
        //THIS DOES NOT LOAD BMP BUT A CUSTOM FORMAT THAT IS JUST BYTES (GRAY SCALE)
        std::ifstream file (path, std::ios::binary);
        file.read(&(data[count][0]), SINGLE_TEXTURE_BYTES);
        ++count;
    }

    void LoadDirectory_RawFormat(chars_t path)
    {
        for(auto const& it : std::filesystem::directory_iterator(path)) { 
            LoadFile_RawFormat(it.path()); //wchar
        }
    }

};

}//ns