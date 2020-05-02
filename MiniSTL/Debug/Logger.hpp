#pragma once
#include "MiniSTL/Debug/Console.hpp" //win 
#include <iostream>

#define DLOG(...) mini::dbg::log(__VA_ARGS__); 
#define ErrLOG(...) mini::dbg::err(__VA_ARGS__); //todo: colored output (os specific)

namespace mini::dbg
{
    template<class... Args>
    void log(Args... args) //todo fmt log
    {
        ((std::cout << args << " " ), ...);
        std::cout << '\n';
    }

    template<class... Args>
    void err(Args... args) //todo fmt log
    {
        std::cout << CONSOLE_RED;
        ((std::cout << args << " "), ...);
        std::cout << CONSOLE_DEFAULT"\n";
    }

}//ns