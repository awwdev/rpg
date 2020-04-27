#pragma once
#include "MiniSTL/Types.hpp"

namespace mini::box 
{
    template<auto BIT_COUNT_T>
    struct Bitset
    {
        static constexpr auto BIT_COUNT  = (u32)(BIT_COUNT_T);
        static constexpr auto BYTE_COUNT = (u32)(BIT_COUNT_T) % 8u ? (u32)BIT_COUNT_T / 8u + 1u : (u32)BIT_COUNT_T / 8u;
        
        u8 data [BYTE_COUNT] = { 0 };


        Bitset(const std::size_t num = 0)
        {
            for (auto i = 0; i < BYTE_COUNT; ++i)
            {
                const auto bits = ((BYTE_COUNT - 1 - i) * 8);
                data[BYTE_COUNT-1-i] = static_cast<u8>((num >> bits) & 0xFFFF); //maybe increase
            }
        }

        template<typename T, typename = IsIntegral<T>> //allowing enum class
        void Set(const T i, const bool b = true)
        {
            if constexpr (std::is_enum_v<T>)
            {
                const auto j = static_cast<std::underlying_type_t<T>>(i);
                data[j / 8] = b
                    ? data[j / 8] | (1ul << (j % 8))
                    : data[j / 8] & ~(1ul << (j % 8));
            }
            else
            {
                data[i / 8] = b
                    ? data[i / 8] | (1ul << (i % 8))
                    : data[i / 8] & ~(1ul << (i % 8));
            }
        }

        template<bool B, typename T, typename = IsIntegral<T>>
        void Set(const T i)
        {
            if constexpr (std::is_enum_v<T>)
            {
                const auto j = static_cast<std::underlying_type_t<T>>(i);
                if constexpr ( B) data[j / 8] |=  (1ul << (j % 8));
                if constexpr (!B) data[j / 8] &= ~(1ul << (j % 8));
            }
            else
            {
                if constexpr ( B) data[i / 8] |=  (1ul << (i % 8));
                if constexpr (!B) data[i / 8] &= ~(1ul << (i % 8));
            }
        }

        template<typename T, typename = IsIntegral<T>>
        void Flip(const T i)
        {
            if constexpr (std::is_enum_v<T>)
            {
                const auto j = static_cast<std::underlying_type_t<T>>(i);
                data[j / 8] ^= 1ul << j % 8;
            }
            else
            {
                data[i / 8] ^= 1ul << i % 8;
            }
        }

        template<typename T, typename = IsIntegral<T>>
        bool Test(const T i) const
        {
            if constexpr (std::is_enum_v<T>) 
            {
                const auto j = static_cast<std::underlying_type_t<T>>(i);
                return (data[j / 8] & (1 << (j % 8))) > 0;
            }
            else
            {
                return (data[i / 8] & (1 << (i % 8))) > 0;
            }
            return true;
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