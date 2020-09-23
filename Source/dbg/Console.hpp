#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef max

namespace rpg::dbg {
    
struct Console
{
    HANDLE pHandle      {};
    HWND   wndHandle    {};

    Console()
    {
        AllocConsole(); //fails if already open?
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(handle, &dwMode);

        dwMode |= ENABLE_PROCESSED_OUTPUT |  ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(handle, dwMode);
    }

    ~Console()
    {
        CloseWindow(wndHandle);
    }
};

}//ns