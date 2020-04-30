#pragma once
#include "MiniSTL/Debug/Assert.hpp"
#include "MiniSTL/Types.hpp"
#include <iostream>

namespace mini::box 
{
    //allows passing integrals and enum (no outer cast needed)(may violate bounds)
    //no use of proxy class that is returned by operator[], just methods are used
    //constexpr used

    #define BOUNDS_CHECK(T, i, size) mini::Assert((T)i >= (T)0 && (T)i < size, "Bitset idx out of bounds");
    #define BOUNDS_CHECK_STATIC(T, n, size) static_assert((T)n >= (T)0 && (T)n < size, "STATIC ASSERT: Bitset idx out of bounds");

    template<auto BITS_T, typename = IsArraySize<BITS_T>>
    struct Bitset
    {
        using CT = IntegralTypeEnum<BITS_T>; //count type

        static constexpr CT BITS  = (CT)BITS_T;
        static constexpr CT BYTES = (CT)BITS_T / (CT)8 + ((CT)BITS_T % (CT)8 ? (CT)1 : (CT)0); //ceil

        u8 data [BYTES] = { 0 };


        constexpr Bitset(const std::size_t num = 0) //todo: test for large numbers
        {
            for (auto i = 0; i < BYTES; ++i)
            {
                const auto bits = ((BYTES - 1 - i) * 8);
                data[BYTES-1-i] = static_cast<u8>((num >> bits) & 0xFF); //wrapping?
            }
        }

        template<typename T>
        inline constexpr CT Bit(const T& i) const  { return (CT)i % (CT)8; } //get bit index of byte index
        
        template<typename T>
        inline constexpr CT Byte(const T& i) const { return (CT)i / (CT)8; } //get byte index of a bit index

        ///SET

        template<typename T, typename = IsIntegralOrEnum<T>> //out of bounds check?
        void Set(const T i, const bool b)
        {
            BOUNDS_CHECK(CT, i, BITS);
            data[Byte(i)] = b
                ? data[Byte(i)] |  (1 << Bit(i))
                : data[Byte(i)] & ~(1 << Bit(i));
        }

        template<bool B, typename T, typename = IsIntegralOrEnum<T>>
        void Set(const T i) 
        {
            BOUNDS_CHECK(CT, i, BITS);
            if constexpr (B) data[Byte(i)] |=  (1 << Bit(i));
            else             data[Byte(i)] &= ~(1 << Bit(i));
        }

        template<auto N, bool B, typename = IsArrayIndex<N>>
        void Set()
        {
            BOUNDS_CHECK_STATIC(CT, N, BITS);
            if constexpr (B) data[Byte(N)] |=  (1 << Bit(N));
            else             data[Byte(N)] &= ~(1 << Bit(N));
        }

        ///FLIP

        template<typename T, typename = IsIntegralOrEnum<T>>
        void Flip(const T i)
        {
            BOUNDS_CHECK(CT, i, BITS);
            data[Byte(i)] ^= 1 << Bit(i);
        }

        template<auto N, typename = IsArrayIndex<N>>
        void Flip()
        {
            BOUNDS_CHECK_STATIC(CT, N, BITS);
            data[Byte(N)] ^= 1 << Bit(N);
        }

        ///Test

        template<typename T, typename = IsIntegralOrEnum<T>>
        bool Test(const T i) const
        {
            BOUNDS_CHECK(CT, i, BITS);
            return data[Byte(i)] & (1ul << Bit(i));
        }

        template<auto N, typename = IsArrayIndex<N>>
        constexpr bool Test() const
        {
            BOUNDS_CHECK_STATIC(CT, N, BITS);
            return data[Byte(N)] & (1 << Bit(N));
        }

        ///other

        constexpr CT FindFirstFreeBit() const
        {
            for (CT i = 0; i < BITS; ++i)
            {
                if (Test(i) == false)
                    return i;
            }
            return u32max;
        }

        ///get number

        std::size_t Decimal() const
        {
            return 42; //todo: 
        }

    };

#undef BOUNDS_CHECK
#undef BOUNDS_CHECK_STATIC

}//ns



//todo: get number of bitset (decimal)
//todo: check ctor