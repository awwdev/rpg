#pragma once
#include "MiniSTL/Math/Vector.hpp"
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

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const mini::math::Vec2<T>& vec)
    {
        os << vec.x << "|" << vec.y;
        return os;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const mini::math::Vec3<T>& vec)
    {
        os << vec.x << "|" << vec.y << "|" << vec.z;
        return os;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const mini::math::Vec4<T>& vec)
    {
        os << vec.x << "|" << vec.y << "|" << vec.z << "|" << vec.w;
        return os;
    }
}