#pragma once
#include "MiniSTL/Types.hpp"

namespace mini::container 
{
    template<u32 BIT_CAPACITY>
    struct Bitset
    {
        static constexpr auto BYTE_CAPACITY = BIT_CAPACITY % 8 ? BIT_CAPACITY / 8 + 1 : BIT_CAPACITY / 8;
        u8 data [BYTE_CAPACITY] = { 0 };
        u8 count = 0; //active bits (capacity is max count)

        Bitset(const std::size_t num = 0)
        {
            for (auto i = 0; i < BYTE_CAPACITY; ++i)
            {
                const auto bits = ((BYTE_CAPACITY - 1 - i) * 8);
                data[BYTE_CAPACITY-1-i] = (num >> bits) & 0xFFFF; //maybe increase
            }
        }

        void Set(const u8 i, const bool b = true)
        {
            data[i / 8] = b
                ? data[i / 8] |  (1ul << (i % 8))
                : data[i / 8] & ~(1ul << (i % 8));
        }

        void Flip(const u8 i)
        {
            data[i / 8] ^= 1ul << i % 8;
        }

        bool Test(const u8 i)
        {
            return (data[i / 8] & (1 << (i % 8))) > 0;
        }
    };

}//ns