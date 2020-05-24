//https://github.com/awwdev

#pragma once

#include "mini/Types.hpp"
#include "mini/Debug/Logger.hpp"
#include "mini/Debug/Assert.hpp"
#include "mini/Memory/Allocator.hpp"
#include <fstream>
#include <filesystem>

namespace mini::res
{
    template<u32 WIDTH, u32 HEIGHT, u32 CHANNELS = 4>
    struct Texture
    {
        static auto constexpr SIZE = WIDTH * HEIGHT * CHANNELS;

        char  fileData [WIDTH * HEIGHT * CHANNELS]; 
        char* texPtr = nullptr;

        char&       operator[](const u32 i)       { return fileData[i]; }
        const char& operator[](const u32 i) const { return fileData[i]; }
    };


    template<u32 WIDTH, u32 HEIGHT>
    inline auto LoadTexture_BMP(chars_t path, mem::BlockPtr<Texture<WIDTH, HEIGHT>>& blockPtr)
    {
        mem::ClaimBlock(blockPtr);
        auto& texture = *blockPtr;

        std::ifstream file(path, std::ios::ate | std::ios::binary);
        mini::Assert(file.is_open(), "cannot open file");

        const auto size = file.tellg();
        file.seekg(std::ios::beg);
        file.read(texture.fileData, size);

        //? BM HEADER CHECK

        if (texture[0] != 'B' && texture[1] == 'M'){
            mini::Assert(false, "unknown bmp format");
        }

        const auto bmSize     = *(mini::u32*)&texture[2];
        const auto bmOffset   = *(mini::u32*)&texture[10];
        const auto bmWidth    = *(mini::u32*)&texture[18];
        const auto bmHeight   = *(mini::u32*)&texture[22];
        const auto bmBitCount = *(mini::u16*)&texture[28]; //32

        mini::Assert(bmWidth == WIDTH && bmHeight == HEIGHT, "bm dimensions do not match");

        texture.texPtr = texture.fileData + bmOffset;
    }

}//ns