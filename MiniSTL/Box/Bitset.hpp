#pragma once
#include "MiniSTL/Debug/Assert.hpp"
#include "MiniSTL/Types.hpp"

namespace mini::box 
{
#define DO_BOUNDS_CHECK 1
#define DC [[nodiscard]]

    //biset can operate with integrals and enums (no outer cast needed)
    //some constexpr methods
    //no exceptions, checks can be toggled

    //todo: make bitset u32 internal ? casts might be unnecessary
    //YES

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
            CheckBounds(i, BITS);
            data[BYTE(i)] = b
                ? data[BYTE(i)] |  (1 << BIT(i))
                : data[BYTE(i)] & ~(1 << BIT(i));
        }

        template<bool B, typename T, typename = IsIntegralOrEnum<T>>
        void Set(const T i)
        {
            CheckBounds(i, BITS);
            if constexpr (B) data[BYTE(i)] |=  (1 << BIT(i));
            else             data[BYTE(i)] &= ~(1 << BIT(i));
        }

        template<auto N, bool B, typename = IsArrayIndex<N>>
        void Set()
        {
            CheckBounds(N, BITS);
            if constexpr (B) data[BYTE(N)] |=  (1 << BIT(N));
            else             data[BYTE(N)] &= ~(1 << BIT(N));
        }

        ///FLIP

        template<typename T, typename = IsIntegralOrEnum<T>>
        void Flip(const T i)
        {
            CheckBounds(i, BITS);
            data[BYTE(i)] ^= 1 << BIT(i);
        }

        template<auto N, typename = IsArrayIndex<N>>
        void Flip()
        {
            CheckBounds(N, BITS);
            data[BYTE(N)] ^= 1 << BIT(N);
        }

        ///Test

        template<typename T, typename = IsIntegralOrEnum<T>> DC
        bool Test(const T i) const
        {
            CheckBounds(i, BITS);
            return data[BYTE(i)] & (1 << BIT(i));
        }

        template<auto N, typename = IsArrayIndex<N>> DC
        constexpr bool Test() const
        {
            CheckBounds(N, BITS);
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

    private:
        template<typename IDX>
        constexpr bool CheckBounds(const IDX i, const COUNT_T size) const
        {
        #if (DO_BOUNDS_CHECK)
            if constexpr (std::is_enum_v<IDX>)
            {
                using UT = std::underlying_type_t<IDX>;
                if ((UT)i < 0 || (UT)i >= size)
                {
                    mini::dbg::dlog<mini::dbg::ColorMode::Red>("Bitset access out of bounds");
                    __debugbreak();
                    return false;
                }
            }
            else
            {
                if (i < 0 || i >= size)
                {
                    mini::dbg::dlog<mini::dbg::ColorMode::Red>("Bitset access out of bounds");
                    __debugbreak();
                    return false;
                }
            }
        #endif

            return true;
        }
    };

#undef DO_BOUNDS_CHECK
#undef BIT
#undef BYTE
#undef DC

}//ns