#pragma once
#include "MiniSTL/Math/Matrix.hpp"
#include "MiniSTL/Box/Bitset.hpp"
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
        std::cout << '\n';
    }

    ///mat

    template<u8 Y, u8 X, class T>
    std::ostream& operator<<(std::ostream& os, const mini::math::Mat<T, Y, X>& mat)
    {
        for (u8 y = 0; y < Y; ++y)
        {
            for (u8 x = 0; x < X; ++x)
            {
                os << mat[y][x] << '|';
            }
            os << '\n';
        }
        return os;
    }


}//ns