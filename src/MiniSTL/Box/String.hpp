//https://github.com/awwdev

/*
## mini::box::String

- fixed size (capacity-based with internal count)
    - auto growth can be harmful on perf (reallocations) and makes it harder to use with custom allocators
    - simple inheritance (without vtable) is used so the (abstract) base can be passed around without the need of writing String<N> everywhere
    - wrapper and elements are not seperated in memory and the whole object can simply be passed to an allocator
- overloads for array, ptrs and the class 
- bounds checking is toggleable via macro, no exceptions are used
- overall more readable and extendible than the STL

*/

#pragma once
#include "MiniSTL/Types.hpp"
#include "MiniSTL/Debug/Logger.hpp"
#include "MiniSTL/Debug/Profiler.hpp"


namespace mini::box
{
#define DO_BOUNDS_CHECK 1
#define ND [[nodiscard]]


    //size agnostic in type - for passing as ref
    template<typename CHAR_T = char>
    struct IString
    {
        using DATA_T = CHAR_T;
        const u32 COUNT_MAX;
        CHAR_T* const dataPtr;

        //? ACCESS

        auto&        operator[](const u32 i)       { CheckBounds(i, count); return dataPtr[i]; }
        auto& const  operator[](const u32 i) const { CheckBounds(i, count); return dataPtr[i]; }

        ND u32  Length() const { return count - 1;  } //potential issue when count == 0
        ND u32  Count()  const { return count;      }
        ND bool Empty()  const { return count == 0; }


        void Clear() 
        { 
            dataPtr[0] = '\0'; 
            count = 1;
        }

        void GetCharArray(char* const arr) const
        {
            std::memcpy(arr, dataPtr, count * sizeof(CHAR_T));
        }


        //? SET

        template<std::size_t N> 
        void Set(const CHAR_T(&arr)[N])
        {
            Set(arr, N); //no need for strlen
        }

        template<class PTR, typename = IsNoArray<PTR>, typename = IsPointer<PTR>>
        void Set(const PTR ptr)
        {
            Set(ptr, std::strlen(ptr) + 1);
        }

        //arrCount has to be strlen + 1
        void Set(const CHAR_T* const ptr, const u32 arrCount)
        {
            CheckBounds(arrCount, COUNT_MAX + 1);
            std::memcpy(dataPtr, ptr, arrCount);
            count = arrCount;
        }


        //? APPEND

        template<std::size_t N>
        void Append(const CHAR_T(&arr)[N])
        {
            Append(arr, N); //no need for strlen
        }
        
        template<class PTR, typename = IsNoArray<PTR>, typename = IsPointer<PTR>>
        void Append(const PTR ptr)
        {
            Append(ptr, strlen(ptr) + 1);
        }
        
        //arrCount has to be strlen + 1
        void Append(const CHAR_T* const ptr, const u32 arrCount)
        {
            //consider the \0 trail
            CheckBounds(count - 1 + arrCount, COUNT_MAX + 1);
            std::memcpy(dataPtr + count - 1, ptr, arrCount * sizeof(CHAR_T));
            count += arrCount - 1;
        }

        void Append(const IString<CHAR_T>& str)
        {
            Append(str.dataPtr, str.count);
        }


        //? REMOVE

        void Remove(const u32 i, const u32 len)
        {
            static_assert(false, "Not yet implemented");
        }


        //? OTHER

        u32 Find()
        {
            static_assert(false, "Not yet implemented");
            return 0;
        }


    protected:
        u32 count; //includes \0

        //? CTOR

        //"abstract" base shall not be instantiated
        constexpr IString(CHAR_T* const data, const u32 countMax, const u32 pCount ) 
            : dataPtr   { data }
            , COUNT_MAX { countMax }
            , count     { pCount } 
        { ; }
        
        //me avoiding boilerplate, use generic methods instead
        IString(const IString&)             = delete;
        IString& operator=(const IString&)  = delete;


        //? INTERNAL

        constexpr inline void CheckBounds(const u32 i, const u32 max) const
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


        //? CTOR

        String() : BASE(data, COUNT_MAX, 1), data { "" } { ; } //is \0

        template<class PTR, typename = IsNoArray<PTR>>
        String(const PTR  ptr) : String() { BASE::Set(ptr); }

        template<std::size_t N>
        String(const CHAR_T (&arr)[N]) : String() { BASE::Set(arr); }

    private:
        CHAR_T data[COUNT_MAX];
    };



    //? HELPER

    template<class... STRINGS>
    auto ConcatStrings(const STRINGS&... strs)
    {
        static_assert("not yet implemented");
    }


    template<typename CHAR_T>
    std::ostream& operator<<(std::ostream& os, const IString<CHAR_T>& str)
    {
        //os << &str[0];
        os.write(str.dataPtr, str.Count()); //should be faster ?
        return os;
    }

}//ns

#undef DO_BOUNDS_CHECK
#undef ND