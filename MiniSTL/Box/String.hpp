//https://github.com/awwdev

/*
## mini::box::String

- fixed size (capacity-based with internal count)
    - auto growth can be harmful on perf (reallocations) and makes it harder to use with custom allocators
    - simple inheritance (without vtable) is used so the (abstract) base can be passed around without the need of writing String<N> everywhere
    - wrapper and elements are not seperated in memory and the whole object can simply be passed to an allocator
- bounds checking is toggleable via macro, no exceptions are used
- overall more readable and extendible than the STL
- template typename CHAR_ARR allows that the size of a char array is carried over to the method

*/

#pragma once
#include "MiniSTL/Types.hpp"
#include "MiniSTL/Debug/Logger.hpp"
#include "MiniSTL/Debug/Profiler.hpp"


namespace mini::box
{
#define DO_BOUNDS_CHECK 1
#define ND [[nodiscard]]


    //size agnostic in type - use to pass string around like IString&
    template<typename CHAR_T = char>
    struct IString
    {
        using DATA_T = CHAR_T;
        const u32 COUNT_MAX;

        CHAR_T* const dataPtr;


        ///ACCESS

        inline auto&        operator[](const u32 i)       { CheckBounds(i, count); return dataPtr[i]; }
        inline auto& const  operator[](const u32 i) const { CheckBounds(i, count); return dataPtr[i]; }

        ND u32  Count() const { return count; }
        ND bool Empty() const { return count == 0; }
           void Clear()       { dataPtr[0] = '\0'; count = 0; }

        void GetCharArray(char* const arr) const
        {
            std::memcpy(arr, dataPtr, (count + 1) * sizeof(CHAR_T));
        }


        ///SET COMPLETE

        template<typename CHAR_ARR, typename = IsArray<CHAR_ARR>>
        void SetFromArray(const CHAR_ARR& chars)
        {
            SetFromPointer(chars, sizeof(CHAR_ARR));
        }

        void SetFromPointer(const CHAR_T* chars)
        {
            SetFromPointer(chars, strlen(chars) + 1);
        }

        void SetFromPointer(const CHAR_T* const chars, const u32 len)
        {
            CheckBounds(len, COUNT_MAX + 1);
            std::memcpy(dataPtr, chars, len);
            count = len - 1;
        }

        //------------wip


        ///APPEND

        template<class CHAR_ARRAY>
        void AppendChars(const CHAR_ARRAY& chars)
        {
            CheckBounds(count + sizeof(CHAR_ARRAY), COUNT_MAX + 1);
            std::memcpy(dataPtr + count, chars, sizeof(CHAR_ARRAY));
            count += sizeof(CHAR_ARRAY) - 1;
        }

        void AppendString(const IString& str)
        {
            CheckBounds(count + str.COUNT_MAX, COUNT_MAX + 1);
            std::memcpy(dataPtr + count, str.dataPtr, str.COUNT_MAX * sizeof(CHAR_T));
            count += str.Count() - 1;
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

    protected:
        u32 count; //does not include \0

        ///CTOR

        constexpr IString(CHAR_T* const data, const u32 countMax, const u32 pCount ) 
            : dataPtr   { data }
            , COUNT_MAX { countMax }
            , count     { pCount } 
        { ; }
        
        //avoid boilerplate, use generic methods instead
        IString(const IString&) = delete;
        IString& operator=(const IString&) = delete;


        ///INTERNAL

        constexpr void CheckBounds(const u32 i, const u32 max) const
        {
        #if (DO_BOUNDS_CHECK)
            if (i < 0 || i >= max)
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

        ///CTORS

        constexpr String() : BASE(data, COUNT_MAX, 0), data { "\0" } { ; }

        template<class CHAR_ARRAY>
        constexpr String(const CHAR_ARRAY& chars) : String() { BASE::Set(chars); }

    private:
        CHAR_T data[COUNT_MAX];
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
        os << &str[0];
        return os;
    }

}//ns

#undef DO_BOUNDS_CHECK
#undef ND