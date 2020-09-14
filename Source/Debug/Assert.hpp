#pragma once
#include "Debug/Logger.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef max

#include <fstream>

namespace rpg::dbg {

inline void DebugBreak()
{
    //win only
    //__debugbreak();
    //BringWindowToTop(GetConsoleWindow()); 
}

template<class T>
inline void Assert(const bool expression, const T& msg) 
{ 
    if (!expression)  {
        dbg::LogError(msg);
        dbg::DebugBreak();
    }
} 

}//NS