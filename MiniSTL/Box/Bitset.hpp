#pragma once
#include "MiniSTL/Debug/Assert.hpp"
#include "MiniSTL/Types.hpp"

namespace mini::box 
{
#define DO_BOUNDS_CHECK 1
#define ND [[nodiscard]]

    template<auto BITS_T, typename CT = u32, typename = IsArraySize<BITS_T>>
    struct Bitset
    {
        using COUNT_T = CT;

        static constexpr CT BITS  = (CT)BITS_T;
        static constexpr CT BYTES = (CT)BITS_T / (CT)8 + ((CT)BITS_T % (CT)8 ? (CT)1 : (CT)0); //ceil

        u8 data [BYTES] = { 0 };


        constexpr Bitset(const std::size_t num = 0)
        {
            for(CT i = 0; i < BYTES - 1; ++i)
            {
                const CT bits = (BYTES - 1 - i) * 8;
                data[BYTES - 1 - i] = static_cast<u8>((num >> bits) & 0xFF); //0xFF prevents wrapping?
            }
        }

        #define BIT(i)  (CT)i % (CT)8
        #define BYTE(i) (CT)i / (CT)8

        ///SET

        template<typename IDX, typename = IsIntegralOrEnum<IDX>>
        void Set(const IDX i, const bool b)
        {
            CheckBounds(i, BITS);
            data[BYTE(i)] = b
                ? data[BYTE(i)] |  (1 << BIT(i))
                : data[BYTE(i)] & ~(1 << BIT(i));
        }

        template<bool B, typename IDX, typename = IsIntegralOrEnum<IDX>>
        void Set(const IDX i)
        {
            CheckBounds(i, BITS);
            if constexpr (B) data[BYTE(i)] |=  (1 << BIT(i));
            else             data[BYTE(i)] &= ~(1 << BIT(i));
        }

        template<auto IDX, bool B, typename = IsArrayIndex<IDX>>
        void Set()
        {
            CheckBounds(IDX, BITS);
            if constexpr (B) data[BYTE(IDX)] |=  (1 << BIT(IDX));
            else             data[BYTE(IDX)] &= ~(1 << BIT(IDX));
        }

        ///FLIP

        template<typename IDX, typename = IsIntegralOrEnum<IDX>>
        void Flip(const IDX i)
        {
            CheckBounds(i, BITS);
            data[BYTE(i)] ^= 1 << BIT(i);
        }

        template<auto IDX, typename = IsArrayIndex<IDX>>
        void Flip()
        {
            CheckBounds(IDX, BITS);
            data[BYTE(IDX)] ^= 1 << BIT(IDX);
        }

        ///Test

        template<typename IDX, typename = IsIntegralOrEnum<IDX>> ND
        bool Test(const IDX i) const
        {
            CheckBounds(i, BITS);
            return data[BYTE(i)] & (1 << BIT(i));
        }

        template<auto IDX, typename = IsArrayIndex<IDX>> ND
        constexpr bool Test() const
        {
            CheckBounds(IDX, BITS);
            return data[BYTE(IDX)] & (1 << BIT(IDX));
        }

        ///other

        ND constexpr CT FindFirstFreeBit() const
        {
            for (CT i = 0; i < BITS; ++i)
            {
                const auto a = data[BYTE(i)] & (1 << BIT(i)); //Test() is due to Assert not constexpr
                if (a == 0) return i;
            }
            return std::numeric_limits<CT>::max();
        }

        ///get number

        ND constexpr std::size_t Decimal() const
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
        constexpr bool CheckBounds(const IDX i, const CT size) const
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
#undef ND

}//ns

/*
RATIONALE

biset can operate with integrals and enums (no outer cast needed)
some constexpr methods
no exceptions, checks can be toggled
*/