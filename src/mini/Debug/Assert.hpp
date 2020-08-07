#pragma once
#include "mini/Debug/Logger.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef max

//win only
#define DEBUG_BREAK() \
    __debugbreak(); \
    BringWindowToTop(GetConsoleWindow()); \

namespace mini {

template<class T>
inline void Assert(const bool expression, const T& msg) 
{ 
    if (!expression)  {
        dbg::LogError(msg);
        //DEBUG_BREAK();
    }
} 

}//ns