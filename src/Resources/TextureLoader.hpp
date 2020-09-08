//https://github.com/awwdev

#pragma once

#include "Utils/Types.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Assert.hpp"

#include <fstream>
#include <filesystem>

namespace rpg::res
{
    
    //TODO: own raw format RGBA
    void LoadTexture(chars_t, char* const)
    {
        //std::ifstream file(path, std::ios::ate | std::ios::binary);
        //rpg::dbg::Assert(file.is_open(), "cannot open file");

        ////!in this function we assuem we know the size upfront
        //const auto size = file.tellg();
        //file.seekg(std::ios::beg);
        //file.read(data, size);

        ////? BM HEADER CHECK

        //if (data[0] != 'B' && data[1] == 'M'){
        //    rpg::dbg::Assert(false, "unknown bmp format");
        //}

        //const auto bmSize     = *(rpg::u32*)&data[2];
        //const auto bmOffset   = *(rpg::u32*)&data[10];
        //const auto bmWidth    = *(rpg::u32*)&data[18];
        //const auto bmHeight   = *(rpg::u32*)&data[22];
        //const auto bmBitCount = *(rpg::u16*)&data[28];


        //rpg::dbg::Assert(bmWidth <= width && bmHeight <= height, "bm dimensions not sufficient");
        //rpg::dbg::Assert(bmBitCount <= channels * 8, "bm channels not sufficient");
        //
        //width    = bmWidth;
        //height   = bmHeight;
        //channels = bmBitCount / 8;
        //texPtr   = filePtr + bmOffset;
    }




    struct ITexture
    {
        ITexture(char* const pFilePtr, const u32 w, const u32 h, const u32 channels, const std::size_t size) 
            : texPtr  { nullptr } //assign when loading
            , filePtr { pFilePtr }
            , width   { w }
            , height  { h }
            , channels{ channels }
            , SIZE    { size }
        {;}

        char* texPtr;
        char* const filePtr;
        u32 width, height, channels;
        const std::size_t SIZE;
        
        //file access (not direct texture access)!
        char&       operator[](const u32 i)       { return filePtr[i]; }
        const char& operator[](const u32 i) const { return filePtr[i]; }

        void LoadFrom_BMP(chars_t path)
        {
            std::ifstream file(path, std::ios::ate | std::ios::binary);
            rpg::dbg::Assert(file.is_open(), "cannot open file");

            const auto size = file.tellg();
            file.seekg(std::ios::beg);
            file.read(filePtr, size);

            //? BM HEADER CHECK

            if (filePtr[0] != 'B' && filePtr[1] == 'M'){
                rpg::dbg::Assert(false, "unknown bmp format");
            }

            //const auto bmSize     = *(rpg::u32*)&filePtr[2];
            const auto bmOffset   = *(rpg::u32*)&filePtr[10];
            const auto bmWidth    = *(rpg::u32*)&filePtr[18];
            const auto bmHeight   = *(rpg::u32*)&filePtr[22];
            const auto bmBitCount = *(rpg::u16*)&filePtr[28];

            rpg::dbg::Assert(bmWidth <= width && bmHeight <= height, "bm dimensions not sufficient");
            rpg::dbg::Assert(bmBitCount <= channels * 8, "bm channels not sufficient");

            width    = bmWidth;
            height   = bmHeight;
            channels = bmBitCount / 8;
            texPtr   = filePtr + bmOffset;
        }
    };

    template<u32 WIDTH_T, u32 HEIGHT_T, u32 CHANNELS_T = 4>
    struct Texture : ITexture
    {
        static auto constexpr MAX_WIDTH    = WIDTH_T;
        static auto constexpr MAX_HEIGHT   = HEIGHT_T;
        static auto constexpr CHANNELS     = CHANNELS_T;
        static auto constexpr SIZE         = MAX_WIDTH * MAX_HEIGHT * CHANNELS;

        char fileData [MAX_WIDTH * MAX_HEIGHT * CHANNELS]; 

        Texture() : ITexture(fileData, MAX_WIDTH, MAX_HEIGHT, CHANNELS, SIZE) {;} 
    };

}//ns