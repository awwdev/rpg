#pragma once
#include "MiniSTL/Math/Vector.hpp"
#include "MiniSTL/Math/Matrix.hpp"
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

    ///vec

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const mini::math::Vec2<T>& vec)
    {
        os << vec.x << '|' << vec.y;
        return os;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const mini::math::Vec3<T>& vec)
    {
        os << vec.x << '|' << vec.y << '|' << vec.z;
        return os;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const mini::math::Vec4<T>& vec)
    {
        os << vec.x << '|' << vec.y << '|' << vec.z << '|' << vec.w;
        return os;
    }

    ///mat

    std::ostream& operator<<(std::ostream& os, const mini::math::Mat4f& mat)
    {
        os << mat[0][0] << '|' << mat[0][1] << '|' << mat[0][2] << '|' << mat[0][3] << '\n';
        os << mat[1][0] << '|' << mat[1][1] << '|' << mat[1][2] << '|' << mat[1][3] << '\n';
        os << mat[2][0] << '|' << mat[2][1] << '|' << mat[2][2] << '|' << mat[2][3] << '\n';
        os << mat[3][0] << '|' << mat[3][1] << '|' << mat[3][2] << '|' << mat[3][3] << '\n';
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const mini::math::Mat3f& mat)
    {
        os << mat[0][0] << '|' << mat[0][1] << '|' << mat[0][2] << '\n';
        os << mat[1][0] << '|' << mat[1][1] << '|' << mat[1][2] << '\n';
        os << mat[2][0] << '|' << mat[2][1] << '|' << mat[2][2] << '\n';
        return os;
    }
}