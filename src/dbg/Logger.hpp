//https://github.com/awwdev

#pragma once

#include <iostream>
#include <cstdio>

namespace rpg::dbg {

constexpr bool IS_LOGGING_INFOS    = true;
constexpr bool IS_LOGGING_WARNINGS = true;
constexpr bool IS_LOGGING_ERRORS   = true;

enum ConsoleColors
{
    White,
    BrightRed,
    Red,
    BrightGreen,
    BrightYellow,
    BrightCyan,
};

template<class... T>
void LogColor(const ConsoleColors color, T&&... args)
{
    switch(color)
    {
        case ConsoleColors::White        : std::cout << "\x1b[0m" ; break;
        case ConsoleColors::BrightRed    : std::cout << "\x1b[91m"; break;
        case ConsoleColors::Red          : std::cout << "\x1b[31m"; break;
        case ConsoleColors::BrightGreen  : std::cout << "\x1b[92m"; break;
        case ConsoleColors::BrightYellow : std::cout << "\x1b[93m"; break;
        case ConsoleColors::BrightCyan   : std::cout << "\x1b[96m"; break;
    }

  ((std::cout << args << " " ), ...);
    std::cout << "\x1b[0m" << '\n'; //white
}

//? COMMON

template<class... T> void LogInfo(T&&... args)
{
    if constexpr(!IS_LOGGING_INFOS) return;
    LogColor(ConsoleColors::White, std::forward<T>(args)...);
}

template<class... T> void LogWarning(T&&... args)
{
    if constexpr(!IS_LOGGING_WARNINGS) return;
    LogColor(ConsoleColors::BrightYellow, std::forward<T>(args)...);
}

template<class... T> void LogError(T&&... args)
{
    if constexpr(!IS_LOGGING_ERRORS) return;
    LogColor(ConsoleColors::BrightRed, std::forward<T>(args)...);
}

} //NS