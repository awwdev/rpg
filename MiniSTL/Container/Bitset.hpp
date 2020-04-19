#pragma once
#include "MiniSTL/Types.hpp"

namespace mini::container 
{
    template<u32 BIT_COUNT_T>
    struct Bitset
    {
        static constexpr auto BIT_COUNT  = BIT_COUNT_T;
        static constexpr auto BYTE_COUNT = BIT_COUNT_T % 8 ? BIT_COUNT_T / 8 + 1 : BIT_COUNT_T / 8;
        
        u8 data [BYTE_COUNT] = { 0 };

        Bitset(const std::size_t num = 0)
        {
            for (auto i = 0; i < BYTE_COUNT; ++i)
            {
                const auto bits = ((BYTE_COUNT - 1 - i) * 8);
                data[BYTE_COUNT-1-i] = (num >> bits) & 0xFFFF; //maybe increase
            }
        }

        void Set(const u32 i, const bool b = true)
        {
            data[i / 8] = b
                ? data[i / 8] |  (1ul << (i % 8))
                : data[i / 8] & ~(1ul << (i % 8));
        }

        template<bool B>
        void Set(const u32 i)
        {
            if constexpr (B)  data[i / 8] | (1ul << (i % 8));
            if constexpr (!B) data[i / 8] & ~(1ul << (i % 8));
        }

        void Flip(const u32 i)
        {
            data[i / 8] ^= 1ul << i % 8;
        }

        bool Test(const u32 i) const
        {
            return (data[i / 8] & (1 << (i % 8))) > 0;
        }

        u32 FindFirstFreeBit() const
        {
            for (u32 i = 0; i < BIT_COUNT; ++i)
            {
                if (Test(i) == false)
                    return i;
            }
            return u32max;
        }

    };

}//ns