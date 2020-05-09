//https://github.com/awwdev

/*
## mini::box::Bitset

- bitset is usable with enums (no cast by the user needed)
- bounds checking is toggleable via macro, no exceptions are used
- some constexpr methods and inlined (templated) methods (like Set<0, true>())
- overall more readable and extendible than the STL
*/

#pragma once
#include "MiniSTL/Types.hpp"
#include "MiniSTL/Debug/Assert.hpp"


namespace mini::box 
{
#define DO_BOUNDS_CHECK 1
#define ND [[nodiscard]]

    template<auto BITS_T, typename IDX_T = u32, typename = IsArraySize<BITS_T>>
    struct Bitset
    {
        using INDEX_T = IDX_T;

        static constexpr IDX_T BITS  = (IDX_T)BITS_T;
        static constexpr IDX_T BYTES = (IDX_T)BITS_T / (IDX_T)8 + ((IDX_T)BITS_T % (IDX_T)8 ? (IDX_T)1 : (IDX_T)0); //ceil

        u8 data [BYTES] = { 0 };

        ///CTOR

        constexpr Bitset(const std::size_t num = 0)
        {
            if (BytesNeeded(num) > BYTES) { //will result in "expression did not evaluate to a constant"(msvc) when constexpr fails
                mini::dbg::dlog<mini::dbg::ColorMode::Red>("Bitset access out of bounds");
                __debugbreak();
            }

            for(IDX_T i = 0; i < BYTES; ++i)
            {
                const IDX_T bits = i * 8;
                data[i] = static_cast<u8>((num >> bits) & 0xFF); //0xFF prevents wrapping?
            }
        }

        #define BIT(i)  (IDX_T)i % (IDX_T)8
        #define BYTE(i) (IDX_T)i / (IDX_T)8

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

        ///TEST

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

        ///OTHER

        ND constexpr IDX_T FindFirstFreeBit() const
        {
            for (IDX_T i = 0; i < BITS; ++i)
            {
                const auto a = data[BYTE(i)] & (1 << BIT(i)); //Test() is due to Assert not constexpr
                if (a == 0) return i;
            }
            return std::numeric_limits<IDX_T>::max();
        }

        ///GET NUMBER

        ND constexpr std::size_t Decimal() const
        {
            std::size_t num = 0;
            for (auto i = 0; i < BYTES; ++i)
            {
                num += (((std::size_t)data[i]) << (i*8));
            }            
            return num;
        }

        constexpr static auto BytesNeeded(std::size_t num)
        {
            int n = 0;
            do { num >>= 8; n++; } while (num);
            return n;
        }

    private:
        template<typename IDX>
        constexpr void CheckBounds(const IDX i, const IDX_T max) const
        {
        #if (DO_BOUNDS_CHECK)
            if constexpr (std::is_enum_v<IDX>)
            {
                using UT = std::underlying_type_t<IDX>;
                if ((UT)i < 0 || (UT)i >= max)
                {
                    mini::dbg::dlog<mini::dbg::ColorMode::Red>("Bitset access out of bounds");
                    __debugbreak();
                }
            }
            else
            {
                if (i < 0 || i >= max)
                {
                    mini::dbg::dlog<mini::dbg::ColorMode::Red>("Bitset access out of bounds");
                    __debugbreak();
                }
            }
        #endif
        }
    };

#undef DO_BOUNDS_CHECK
#undef BIT
#undef BYTE
#undef ND

}//ns