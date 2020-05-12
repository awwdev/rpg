#pragma once
#include "mini/Debug/Logger.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef max

namespace mini 
{
    inline void Assert(const bool b, const char* msg = "") 
    { 
        if (!b)
        {
            BringWindowToTop(GetConsoleWindow()); //win only
            mini::dbg::dlog<mini::dbg::ColorMode::Red>(msg);
            system("pause"); //win only
        }
    } 

}//ns
