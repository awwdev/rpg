#pragma once
#include <iostream>

#define DLOG(...) mini::debug::log(__VA_ARGS__); 

namespace mini::debug
{
    template<class... Args>
    void log(Args... args) //todo fmt log
    {
        ((std::cout << args << " " ), ...);
        std::cout << '\n';
    }
}