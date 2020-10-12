//https://github.com/awwdev

#pragma once
#include "com/Utils.hpp"
#include "dbg/Assert.hpp"
#include "dbg/Logger.hpp"

namespace rpg::com::mem {

template<auto BYTE_CAPACITY_T>
struct BinaryMemory
{
    static constexpr idx_t BYTE_CAPACITY = static_cast<idx_t>(BYTE_CAPACITY_T);
    char data [BYTE_CAPACITY];
    idx_t byteIdx = 0;

    template<typename T>
    void operator<<(T const& pData)
    {
        //TODO: compress
        std::memcpy(&data[byteIdx], &pData, sizeof(T));
        byteIdx += sizeof(T);
        dbg::Assert(byteIdx < BYTE_CAPACITY && byteIdx >= 0, "[BinaryBlob] idx out of bounds");
    }

    template<typename T>
    void operator>>(T& pData)
    {
        //TODO: decompress
        std::memcpy(&pData, &data[byteIdx], sizeof(T));
        byteIdx -= sizeof(T);
        dbg::Assert(byteIdx < BYTE_CAPACITY && byteIdx >= 0, "[BinaryBlob] idx out of bounds");
    }

    void Write(char const* const pData, std::size_t const size)
    {
        //TODO: compress
        std::memcpy(&data[byteIdx], &pData, size);
        byteIdx += size;
        dbg::Assert(byteIdx < BYTE_CAPACITY && byteIdx >= 0, "[BinaryBlob] idx out of bounds");
    }

    void Read(char* const pData, std::size_t const size)
    {
        //TODO: decompress
        std::memcpy(&pData, &data[byteIdx], size);
        byteIdx -= size;
        dbg::Assert(byteIdx < BYTE_CAPACITY && byteIdx >= 0, "[BinaryBlob] idx out of bounds");
    }

};

}//ns