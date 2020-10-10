//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "dbg/Assert.hpp"
#include "dbg/Logger.hpp"
#include "com/box/Optional.hpp"

#include <cstring>
#include <fstream>

namespace rpg::com {

#define FOR_BITSET(bitset, i) for(idx_t i = 0; i < bitset.BITS; ++i)

constexpr bool USE_BITSET_ASSERTS = true;

inline void BitsetAssert(const bool condition, chars_t msg = "bitset assertion failed")
{
    if constexpr(USE_BITSET_ASSERTS) {
        dbg::Assert(condition, msg);
    }
}

//TODO: as function instead of macro:
#define BIT(i)  (idx_t)i % (idx_t)8
#define BYTE(i) (idx_t)i / (idx_t)8

constexpr idx_t BytesNeeded(idx_t num)
{
    int n = 0;
    do { num >>= 8; n++; } while (num);
    return n;
}

template<auto N>
struct Bitset
{
    //? DATA

    static constexpr idx_t BITS  = (idx_t)N;
    static constexpr idx_t BYTES = BITS/8 + 1;
    u8 bytes [BYTES] = { 0 };

    constexpr Bitset(const idx_t num = 0)
    {
        BitsetAssert(BytesNeeded(num) <= BYTES, "bitset ctor input to big");

        for(idx_t i = 0; i < BYTES; ++i) {
            const idx_t bits = i * 8;
            bytes[i] = static_cast<u8>((num >> bits) & 0xFF); //0xFF prevents wrapping?
        }
    }

    //? MODIFICATION

    template<class IDX> 
    void Set(const IDX i, const bool setTrue = true)
    {
        BitsetAssert((idx_t)i < BITS);
        bytes[BYTE(i)] = setTrue
            ? bytes[BYTE(i)] |  (1 << BIT(i))
            : bytes[BYTE(i)] & ~(1 << BIT(i));
    }

    template<class IDX> 
    bool Test(const IDX i) const
    {
        BitsetAssert((idx_t)i < BITS);
        return bytes[BYTE(i)] & (1 << BIT(i));
    }

    template<class IDX>
    void Flip(const IDX i)
    {
        BitsetAssert(static_cast<idx_t>(i) < BITS);
        bytes[BYTE(i)] ^= 1 << BIT(i);
    }

    void Clear()
    {
        std::memset(bytes, 0, BYTES);
    }

    //? HELPER

    com::Optional<idx_t> FindFreeBitOptional(const idx_t startAt = 0) const
    {
        for (idx_t i = startAt; i < BITS; ++i) {
            const auto a = bytes[BYTE(i)] & (1 << BIT(i)); //Test() is due to Assert not constexpr
            if (a == 0) 
                return { i };
        }
        return {};
    }

    constexpr auto Decimal() const
    {
        std::size_t num = 0;
        for (auto i = 0; i < BYTES; ++i)
        {
            num += (((std::size_t)bytes[i]) << (i*8));
        }            
        return num;
    }

    //? serialization

    void WriteBinaryFile(chars_t path) const
    {
        std::ofstream file { path, std::ios::binary };
        dbg::Assert(file.is_open(), "[IO] cannot open file"); //not an array assert
        file.write(reinterpret_cast<char const*>(bytes), BYTES);
    }

    void ReadBinaryFile(chars_t path)
    {
        std::ifstream file { path, std::ios::binary };
        dbg::Assert(file.is_open(), "[IO] cannot open file"); //not an array assert
        file.read(reinterpret_cast<char*>(bytes), BYTES);
    }
};

#undef BIT
#undef BYTE

}//ns