//https://github.com/awwdev

/*
## mini::box::String

- bounds checking is toggleable via macro, no exceptions are used
- overall more readable and extendible than the STL
*/

#pragma once
#include "MiniSTL/Types.hpp"
#include "MiniSTL/Debug/Logger.hpp"


namespace mini::box
{
#define DO_BOUNDS_CHECK 1


    //size agnostic in type - use to pass string around like IString&
    template<typename CHAR_T = char>
    struct IString
    {
        using DATA_T = CHAR_T;
        const u32 COUNT_MAX;


        ///ACCESS

        CHAR_T* const dataPtr = nullptr;
        
        inline auto&        operator[](const u32 i)       { CheckBounds(); return dataPtr[i]; }
        inline auto& const  operator[](const u32 i) const { CheckBounds(); return dataPtr[i]; }


        ///MOD

        void Clear() { dataPtr[0] = '\0'; count = 0; }


        ///SET

        template<class CHAR_ARR>
        constexpr void Set(const CHAR_ARR& str) //possibly called by ctor so constexpr
        {
            CheckBounds(count + sizeof(CHAR_ARR), COUNT_MAX + 1);
            //memcpy and std::copy are not constexpr?
            for (u32 i = 0; i < sizeof(CHAR_ARR); ++i) {
                dataPtr[i] = str[i];
            }
            count = sizeof(CHAR_ARR) - 1;
        }


        ///APPEND

        template<class CHAR_ARR>
        void AppendChars(const CHAR_ARR& chars)
        {
            //CheckBounds();
            std::memcpy(dataPtr + count, chars, sizeof(CHAR_ARR));
            count += sizeof(CHAR_ARR) - 1;
        }

        void AppendString(const IString& str)
        {
            static_assert(false, "Not yet implemented");
        }


        ///REMOVE

        void Remove()
        {
            static_assert(false, "Not yet implemented");
        }


        ///OTHER

        bool Contains()
        {
            static_assert(false, "Not yet implemented");
            return true;
        }


        IString(const IString& str)        { }
        void operator=(const IString& str) { }

    protected:
        u32 count = 0; //does not include \0

        ///CTOR

        constexpr IString(CHAR_T* const data, const u32 countMax) : dataPtr{ data }, COUNT_MAX { countMax } { ; }
        
        ///INTERNAL

        constexpr void CheckBounds(const u32 i, const u32 size) const
        {
        #if (DO_BOUNDS_CHECK)
            if (i < 0 || i >= size)
            {
                mini::dbg::dlog<mini::dbg::ColorMode::Red>("String access out of bounds");
                __debugbreak();
            }
        #endif
        }
    };


    template<u32 COUNT_MAX_T, typename CHAR_T = char>
    struct String final : IString<CHAR_T>
    {
        using DATA_T = CHAR_T;
        using BASE   = IString<CHAR_T>;

        constexpr static u32 COUNT_MAX = COUNT_MAX_T;

        ///CTOR

        constexpr String() : BASE(data, COUNT_MAX) {;}

        template<class CHAR_ARR>
        constexpr String(const CHAR_ARR& chars) : String() { BASE::Set(chars); }

        String(const String& str) : String() { IString(str); }
        void operator=(const String& str)    { BASE::operator=(str); }

    private:
        CHAR_T data[COUNT_MAX] = { 0 };
    };


    ///OTHER

    template<class... STRINGS>
    auto ConcatStrings(const STRINGS&... strs)
    {
        static_assert("not yet implemented");
    }


    template<typename CHAR_T>
    std::ostream& operator<<(std::ostream& os, const IString<CHAR_T>& str)
    {
        os << str.dataPtr;
        return os;
    }

}//ns

#undef DO_BOUNDS_CHECK