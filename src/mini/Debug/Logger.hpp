//https://github.com/awwdev

#pragma once

#include <iostream>
#include <cstdio>

namespace mini::dbg {

constexpr bool LOG_ERRORS   = true;
constexpr bool LOG_WARNINGS = true;
constexpr bool LOG_INFO     = true;

//windows console 
constexpr auto CONSOLE_DEFAULT = "\x1b[0m" ;
constexpr auto CONSOLE_RED     = "\x1b[91m";
constexpr auto CONSOLE_GREEN   = "\x1b[92m";
constexpr auto CONSOLE_YELLOW  = "\x1b[93m";
constexpr auto CONSOLE_CYAN    = "\x1b[96m";

template<class... T>
inline void LogInfo(T&&... args)
{
    if constexpr(!LOG_INFO) return;

    std::cout << CONSOLE_DEFAULT;
    ((std::cout << args << " " ), ...);
    std::cout << CONSOLE_DEFAULT << '\n';
}

template<class... T>
inline void LogHighlight(T&&... args)
{
    if constexpr(!LOG_INFO) return;

    std::cout << CONSOLE_CYAN;
    ((std::cout << args << " " ), ...);
    std::cout << CONSOLE_DEFAULT << '\n';
}

template<class... T>
inline void LogWarning(T&&... args)
{
    if constexpr(!LOG_WARNINGS) return;

    std::cout << CONSOLE_YELLOW;
    ((std::cout << args << " " ), ...);
    std::cout << CONSOLE_DEFAULT << '\n';
}

template<class... T>
inline void LogError(T&&... args)
{
    if constexpr(!LOG_ERRORS) return;

    std::cout << CONSOLE_RED;
    ((std::cout << args << " " ), ...);
    std::cout << CONSOLE_DEFAULT << '\n';
}

}//ns