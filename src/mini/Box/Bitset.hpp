//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"
#include "mini/Debug/Assert.hpp"

namespace mini::box {

#define FOR_BITSET(bitset, i) for(idx_t i = 0; i < bitset.BITS; ++i)

constexpr bool USE_BITSET_ASSERTS = true;

inline void BitsetAssert(const bool condition, chars_t msg = "bitset assertion failed")
{
    if constexpr(USE_BITSET_ASSERTS) {
        if (condition == false){
            dbg::LogError(msg);
            DEBUG_BREAK();
        }
    }
}

#define BIT(i)  (idx_t)i % (idx_t)8
#define BYTE(i) (idx_t)i / (idx_t)8

constexpr auto BytesNeeded(std::size_t num)
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
    u8 data [BYTES] = { 0 };

    constexpr Bitset(const std::size_t num = 0)
    {
        BitsetAssert(BytesNeeded(num) <= BYTES, "bitset ctor input to big");

        for(idx_t i = 0; i < BYTES; ++i) {
            const idx_t bits = i * 8;
            data[i] = static_cast<u8>((num >> bits) & 0xFF); //0xFF prevents wrapping?
        }
    }

    //? MODIFICATION

    template<class IDX> 
    void Set(const IDX i, const bool setTrue = true)
    {
        BitsetAssert((idx_t)i < BITS);
        data[BYTE(i)] = setTrue
            ? data[BYTE(i)] |  (1 << BIT(i))
            : data[BYTE(i)] & ~(1 << BIT(i));
    }

    template<class IDX> 
    bool Test(const IDX i) const
    {
        BitsetAssert((idx_t)i < BITS);
        return data[BYTE(i)] & (1 << BIT(i));
    }

    template<class IDX>
    void Flip(const IDX i)
    {
        BitsetAssert((idx_t)i < BITS);
        data[BYTE(i)] ^= 1 << BIT(i);
    }

    void Clear()
    {
        std::memset(data, 0, BYTES);
    }

    //? HELPER

    constexpr idx_t FindFirstFreeBit(const idx_t startAt = 0) const
    {
        for (idx_t i = startAt; i < BITS; ++i) {
            const auto a = data[BYTE(i)] & (1 << BIT(i)); //Test() is due to Assert not constexpr
            if (a == 0) return i;
        }
        return BITS;
    }

    constexpr auto Decimal() const
    {
        std::size_t num = 0;
        for (auto i = 0; i < BYTES; ++i)
        {
            num += (((std::size_t)data[i]) << (i*8));
        }            
        return num;
    }
};

#undef BIT
#undef BYTE

}//ns