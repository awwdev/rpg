//https://github.com/awwdev
#pragma once

#include "Utils/Types.hpp"
#include <fstream>
#include <filesystem>

namespace rpg::res {
    
template<u32 N, u32 W, u32 H, u8 C = 4>
struct TextureArray
{
    constexpr static auto WIDTH      = W;
    constexpr static auto HEIGHT     = H;
    constexpr static auto CHANNELS   = C;
    constexpr static auto COUNT      = N;
    constexpr static auto TEX_SIZE   = W*H*C;
    constexpr static auto TOTAL_SIZE = TEX_SIZE * N;

    char data [COUNT][TEX_SIZE];
    u32 count = 0;

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

}//ns