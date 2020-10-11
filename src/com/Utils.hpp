//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "dbg/Assert.hpp"
#include <fstream>

namespace rpg::com {

template<typename T, auto N>
void WriteBinaryFile_C_Array(chars_t path, T const (&arr) [N])
{
    auto file = std::ofstream(path, std::ios::binary);
    dbg::Assert(file.is_open(), "[IO] cannot open file");
    file.write(reinterpret_cast<char const*>(arr), N * sizeof(T));
}

template<typename T, auto N>
void ReadBinaryFile_C_Array(chars_t path, T (&arr) [N])
{
    auto file = std::ifstream(path, std::ios::binary);
    dbg::Assert(file.is_open(), "[IO] cannot open file");
    file.read(reinterpret_cast<char*>(arr), N * sizeof(T));
}

}//ns