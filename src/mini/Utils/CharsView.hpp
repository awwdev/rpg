//https://github.com/awwdev

#pragma once
#include "mini/Utils/Types.hpp"

namespace mini::utils
{
    struct CharsView
    {
        chars_t   beginPtr;
        const u32 count;
    };

    inline bool CharsCompare(const CharsView& view, chars_t chars)
    {   
        for(u32 i = 0; i < view.count; ++i)
        {
            if (*(view.beginPtr + i) != *(chars + i))
                return false;
        }
        return true;
    }

    //TODO: more variants: view view, chars chars, str view, ...
    //TODO: maybe rename the header to CharUtils.hpp

}//ns