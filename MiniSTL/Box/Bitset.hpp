#pragma once
#include "MiniSTL/Debug/Assert.hpp"
#include "MiniSTL/Types.hpp"

namespace mini::box 
{
#define BOUNDS_CHECK(t, i, size)    mini::Assert((t)i >= 0 && (t)i < size, "Bitset access out of bounds");
#define BOUNDS_CHECK_S(t, i, size) static_assert((t)i >= 0 && (t)i < size, "Bitset access out of bounds");
#define DC [[nodiscard]]

    //biset can operate with integrals and enums (no outer cast needed)
    //some constexpr methods
    //no exceptions, checks can be toggled

    template<auto BITS_T, typename = IsArraySize<BITS_T>>
    struct Bitset
    {
        using COUNT_T = IntegralTypeEnum<BITS_T>; //count type

        static constexpr COUNT_T BITS  = (COUNT_T)BITS_T;
        static constexpr COUNT_T BYTES = (COUNT_T)BITS_T / (COUNT_T)8 + ((COUNT_T)BITS_T % (COUNT_T)8 ? (COUNT_T)1 : (COUNT_T)0); //ceil

        u8 data [BYTES] = { 0 };


        constexpr Bitset(const std::size_t num = 0)
        {
            for (auto i = BYTES - 1; i >= 0 ; --i)
            {
                const auto bits = (i * 8);
                data[i] = static_cast<u8>((num >> bits) & 0xFF); //0xFF prevents wrapping?
            }
        }

        #define BIT(i)  (COUNT_T)i % (COUNT_T)8
        #define BYTE(i) (COUNT_T)i / (COUNT_T)8

        ///SET

        template<typename T, typename = IsIntegralOrEnum<T>>
        void Set(const T i, const bool b)
        {
            BOUNDS_CHECK(COUNT_T, i, BITS);
            data[BYTE(i)] = b
                ? data[BYTE(i)] |  (1 << BIT(i))
                : data[BYTE(i)] & ~(1 << BIT(i));
        }

        template<bool B, typename T, typename = IsIntegralOrEnum<T>>
        void Set(const T i)
        {
            BOUNDS_CHECK(COUNT_T, i, BITS);
            if constexpr (B) data[BYTE(i)] |=  (1 << BIT(i));
            else             data[BYTE(i)] &= ~(1 << BIT(i));
        }

        template<auto N, bool B, typename = IsArrayIndex<N>>
        void Set()
        {
            BOUNDS_CHECK_S(COUNT_T, N, BITS);
            if constexpr (B) data[BYTE(N)] |=  (1 << BIT(N));
            else             data[BYTE(N)] &= ~(1 << BIT(N));
        }

        ///FLIP

        template<typename T, typename = IsIntegralOrEnum<T>>
        void Flip(const T i)
        {
            BOUNDS_CHECK(COUNT_T, i, BITS);
            data[BYTE(i)] ^= 1 << BIT(i);
        }

        template<auto N, typename = IsArrayIndex<N>>
        void Flip()
        {
            BOUNDS_CHECK_S(COUNT_T, N, BITS);
            data[BYTE(N)] ^= 1 << BIT(N);
        }

        ///Test

        template<typename T, typename = IsIntegralOrEnum<T>> DC
        bool Test(const T i) const
        {
            BOUNDS_CHECK(COUNT_T, i, BITS);
            return data[BYTE(i)] & (1 << BIT(i));
        }

        template<auto N, typename = IsArrayIndex<N>> DC
        constexpr bool Test() const
        {
            BOUNDS_CHECK_S(COUNT_T, N, BITS);
            return data[BYTE(N)] & (1 << BIT(N));
        }

        ///other

        DC constexpr COUNT_T FindFirstFreeBit() const
        {
            for (COUNT_T i = 0; i < BITS; ++i)
            {
                const auto a = data[BYTE(i)] & (1 << BIT(i)); //Test() is due to Assert not constexpr
                if (a == 0) return i;
            }
            return std::numeric_limits<COUNT_T>::max();
        }

        ///get number

        DC constexpr std::size_t Decimal() const
        {
            std::size_t num = 0;
            for (auto i = 0; i < BYTES; ++i)
            {
                num += (((std::size_t)data[i]) << (i*8));
            }            
            return num;
        }

    };

#undef BOUNDS_CHECK
#undef BOUNDS_CHECK_S
#undef BIT
#undef BYTE
#undef DC

}//ns