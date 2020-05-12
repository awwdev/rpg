#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef max

namespace mini::dbg
{
    struct Console
    {
        HANDLE pHandle;
        HWND   wndHandle;

        ~Console()
        {
            CloseWindow(wndHandle);
        }
    };

    [[nodiscard]]
    inline Console CreateConsole() 
    {
        AllocConsole(); //fails if already open?
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(handle, &dwMode);

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(handle, dwMode);

        std::cout.sync_with_stdio(false);//?
        
        return { handle, GetConsoleWindow() }; //avoid dtor 
    }

}//ns