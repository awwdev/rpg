#pragma once
#include "MiniSTL/Types.hpp"
#include "MiniSTL/Debug/Logger.hpp"

namespace mini::box
{
    template<typename CHAR_T>
    struct IString
    {
        u32 count = 0;

        ///MOD

        void Clear()  { dataPtr[0] = '\0'; }
        void Erase()  {}

        inline auto&        operator[](const u32 i)         { return dataPtr[i]; }
        inline auto& const  operator[](const u32 i) const   { return dataPtr[i]; }

        void operator=(const CHAR_T chars[])
        {
            for (u32 i = 0; ; ++i)
            {
                if ((dataPtr[i] = chars[i]) == '\0')
                    break;
            }
        }

        void operator+(const CHAR_T chars[])
        {

        }
        
    protected:
        IString(CHAR_T* const data) : dataPtr { data } { ; }
        CHAR_T* dataPtr;
    };

    template<u32 COUNT_MAX_T, typename CHAR_T = char>
    struct String : public IString<CHAR_T>
    {
        using DATA_T = CHAR_T;
        constexpr static u32 COUNT_MAX = COUNT_MAX_T;

        ///CTOR

        String(const CHAR_T chars[] = "") : IString<CHAR_T>(data) { IString<CHAR_T>::operator=(chars); }

        ///DATA ARRAY

        CHAR_T data[COUNT_MAX_T] = {};
    };


    template<class... STRINGS>
    auto ConcatStrings(const STRINGS&... strs)
    {
        return nullptr; //todo
    }

}//ns