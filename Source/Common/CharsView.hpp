//https://github.com/awwdev

#pragma once
#include "Common/Types.hpp"

namespace rpg::com {

struct CharsView
{
    const char* beginPtr;
    idx_t len;
};

inline bool CharsCompare(const CharsView& view, chars_t chars)
{   
    for(idx_t i = 0; i < view.len; ++i)
    {
        if (*(view.beginPtr + i) != *(chars + i))
            return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const CharsView& view)
{
    os.write(view.beginPtr, view.len);
    return os;
}
}//NS