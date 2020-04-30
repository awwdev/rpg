#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace mini::dbg
{

//win codes
#define CONSOLE_RED    "\x1b[91m"
#define CONSOLE_GREEN  "\x1b[92m"
#define CONSOLE_YELLOW "\x1b[103m"

    inline void SetupConsole()
    {
        AllocConsole();
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }

}//ns