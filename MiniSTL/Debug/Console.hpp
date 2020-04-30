#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace mini::dbg
{
    //win codes
    #define CONSOLE_RED    "\x1b[91m"
    #define CONSOLE_GREEN  "\x1b[92m"
    #define CONSOLE_YELLOW "\x1b[103m"

    struct Console
    {
        HANDLE pHandle;
        HWND   wndHandle;

        ~Console()
        {
            CloseWindow(wndHandle);
        }
    };

    inline Console SetupConsole()
    {
        AllocConsole(); //fails if already open?
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(handle, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(handle, dwMode);

        return { handle, GetConsoleWindow() }; //avoid dtor 
    }

}//ns