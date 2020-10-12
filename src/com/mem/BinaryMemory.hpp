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

    void ResetByteIdx()
    {
        byteIdx = 0;
    }

    template<typename T>
    void operator<<(T const& src)
    {
        std::memcpy(&data[byteIdx], &src, sizeof(T));
        byteIdx += sizeof(T);
        dbg::Assert(byteIdx < BYTE_CAPACITY && byteIdx >= 0, "[BinaryBlob] idx out of bounds");
    }

    template<typename T>
    void operator>>(T& dst)
    {
        std::memcpy(&dst, &data[byteIdx], sizeof(T));
        byteIdx += sizeof(T);
        dbg::Assert(byteIdx < BYTE_CAPACITY && byteIdx >= 0, "[BinaryBlob] idx out of bounds");
    }

    template<typename T>
    void Write(T const* const pSrc, std::size_t const size)
    {
        char const* const src = reinterpret_cast<char const*>(pSrc);
        std::memcpy(&data[byteIdx], &src, size);
        byteIdx += size;
        dbg::Assert(byteIdx < BYTE_CAPACITY && byteIdx >= 0, "[BinaryBlob] idx out of bounds");
    }

    template<typename T>
    void Read(T* pDst, std::size_t const size)
    {
        char* dst = reinterpret_cast<char*>(pDst);
        std::memcpy(&dst, &data[byteIdx], size);
        byteIdx += size;
        dbg::Assert(byteIdx < BYTE_CAPACITY && byteIdx >= 0, "[BinaryBlob] idx out of bounds");
    }

private:
    char data [BYTE_CAPACITY];
    idx_t byteIdx = 0;

};

}//ns