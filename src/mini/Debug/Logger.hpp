#pragma once
#include "mini/Box/String.hpp"
#include <iostream>
#include <cstdio>


namespace mini::dbg
{
#define DO_DLOG 1
#define LOG(...)  mini::dbg::dlog(__VA_ARGS__)
#define LOG_VAR(var)  mini::dbg::dlog(#var, var)

#define INFO(...) mini::dbg::dlog<mini::dbg::ColorMode::Cyan>(__VA_ARGS__)
#define ERR(...)  mini::dbg::dlog<mini::dbg::ColorMode::Red>(__VA_ARGS__)
#define WARN(...) mini::dbg::dlog<mini::dbg::ColorMode::Yellow>(__VA_ARGS__)

//os specfic colors
//windows
#define CONSOLE_DEFAULT "\x1b[0m"
#define CONSOLE_RED     "\x1b[91m"
#define CONSOLE_GREEN   "\x1b[92m"
#define CONSOLE_YELLOW  "\x1b[93m"
#define CONSOLE_CYAN    "\x1b[96m"

//macro DLOG has the adventage of getting the line and the file as macro too (add as macro fn?)

    enum class ColorMode
    {
        Default, Red, Green, Yellow, Cyan
    };

    template<ColorMode CM = ColorMode::Default, class... Args>
    void dlog(const Args&... args)
    {
    #if (DO_DLOG == 1)
        []() constexpr {
            switch (CM)
            {
            case ColorMode::Default: std::cout << CONSOLE_DEFAULT;  break;
            case ColorMode::Red:     std::cout << CONSOLE_RED;      break;
            case ColorMode::Green:   std::cout << CONSOLE_GREEN;    break;
            case ColorMode::Yellow:  std::cout << CONSOLE_YELLOW;   break;
            case ColorMode::Cyan:    std::cout << CONSOLE_CYAN;     break;
            }
        }();

        ((std::cout << args << " " ), ...);
        std::cout << CONSOLE_DEFAULT"\n";
    #endif
    }


#undef CONSOLE_DEFAULT
#undef CONSOLE_RED
#undef CONSOLE_GREEN
#undef CONSOLE_YELLOW

#undef DO_DLOG

}//ns

//todo: more advanced fmt logging?
//todo: store logs external