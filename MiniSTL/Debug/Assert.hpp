#pragma once
#include "MiniSTL/Debug/Logger.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>

namespace mini {
    inline auto Assert(const bool b, const char* msg = "") 
    { 
        if (!b) { 
            BringWindowToTop(GetConsoleWindow()); //win only
            ErrLOG(msg);
            system("pause"); //win only
        } 
    } 
}