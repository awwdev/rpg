#pragma once
#include "MiniSTL/Types.hpp"
#include <cmath>

namespace mini::math
{
    template<class T>
    using IsArithmetic = std::enable_if_t<std::is_arithmetic_v<T>>;

    template<class T>
    using IsFloating = std::enable_if_t<std::is_floating_point_v<T>>;

    template<class T, typename = IsArithmetic<T>> struct Vec2 { T x, y; };
    template<class T, typename = IsArithmetic<T>> struct Vec3 { T x, y, z; };
    template<class T, typename = IsArithmetic<T>> struct Vec4 { T x, y, z, w; };

    ///+

    template<class A, class B>
    [[nodiscard]] auto operator+(const Vec2<A>& a, const Vec2<B>& b) -> Vec2<decltype(a.x + b.x)>
    {
        return { a.x + b.x, a.y + b.y };
    }

    template<class A, class B>
    [[nodiscard]] auto operator+(const Vec3<A>& a, const Vec3<B>& b) -> Vec3<decltype(a.x + b.x)>
    {
        return { a.x + b.x, a.y + b.y, a.z + b.z };
    }

    template<class A, class B>
    [[nodiscard]] auto operator+(const Vec4<A>& a, const Vec4<B>& b) -> Vec4<decltype(a.x + b.x)>
    {
        return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
    }

    ///+=

    template<class A, class B>
    void operator+=(Vec2<A>& a, const Vec2<B>& b)
    {
        a.x += b.x;
        a.y += b.y;
    }

    template<class A, class B>
    void operator+=(Vec3<A>& a, const Vec3<B>& b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
    }

    template<class A, class B>
    void operator+=(Vec4<A>& a, const Vec4<B>& b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
        a.w += b.w;
    }

    ///-

    template<class A, class B>
    [[nodiscard]] auto operator-(const Vec2<A>& a, const Vec2<B>& b) -> Vec2<decltype(a.x - b.x)>
    {
        return { a.x - b.x, a.y - b.y };
    }

    template<class A, class B>
    [[nodiscard]] auto operator-(const Vec3<A>& a, const Vec3<B>& b) -> Vec3<decltype(a.x - b.x)>
    {
        return { a.x - b.x, a.y - b.y, a.z - b.z };
    }

    template<class A, class B>
    [[nodiscard]] auto operator-(const Vec4<A>& a, const Vec4<B>& b) -> Vec4<decltype(a.x - b.x)>
    {
        return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
    }

    ///-=

    template<class A, class B>
    void operator-=(Vec2<A>& a, const Vec2<B>& b)
    {
        a.x -= b.x;
        a.y -= b.y;
    }

    template<class A, class B>
    void operator-=(Vec3<A>& a, const Vec3<B>& b)
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;
    }

    template<class A, class B>
    void operator-=(Vec4<A>& a, const Vec4<B>& b)
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;
        a.w -= b.w;
    }

    ///magnitude
    //todo: faster sqrt method like quake sqrt
    template<class T> 
    [[nodiscard]] auto Magnitude(const Vec2<T>& vec)
    {
        return std::sqrtf(vec.x * vec.x + vec.y * vec.y);
    }

    template<class T> 
    [[nodiscard]] auto Magnitude(const Vec3<T>& vec)
    {
        return std::sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    }

    template<class T> 
    [[nodiscard]] auto Magnitude(const Vec4<T>& vec)
    {
        return std::sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
    }

    ///normalize

    template<class T, typename F = float>
    [[nodiscard]] Vec2<F> Normalize(const Vec2<T>& vec)
    {
        const auto mag = Magnitude(vec);
        return { vec.x / mag, vec.y / mag };
    }

    template<class T, typename F = float>
    [[nodiscard]] Vec3<F> Normalize(const Vec3<T>& vec)
    {
        const auto mag = Magnitude(vec);
        return { vec.x / mag, vec.y / mag, vec.z / mag };
    }

    template<class T, typename F = float>
    [[nodiscard]] Vec4<F> Normalize(const Vec4<T>& vec)
    {
        const auto mag = Magnitude(vec);
        return { vec.x / mag, vec.y / mag, vec.z / mag, vec.w / mag };
    }

    ///normalize this   
    
    template<class T, typename = IsFloating<T>> 
    void NormalizeThis(Vec2<T>& vec)
    {
        const auto mag = Magnitude(vec);
        vec = { vec.x / mag, vec.y / mag };
    }

    template<class T, typename = IsFloating<T>>
    void NormalizeThis(Vec3<T>& vec)
    {
        const auto mag = Magnitude(vec);
        vec = { vec.x / mag, vec.y / mag, vec.z / mag };
    }

    template<class T, typename = IsFloating<T>>
    void NormalizeThis(Vec4<T>& vec)
    {
        const auto mag = Magnitude(vec);
        vec = { vec.x / mag, vec.y / mag, vec.z / mag, vec.w / mag };
    }

    ///dot

    template<class A, class B> 
    [[nodiscard]] float Dot(const Vec3<A>& a, const Vec3<B>& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    ///cross

    template<class A, class B>
    [[nodiscard]] auto Cross(const Vec3<A>& a, const Vec3<B>& b) -> Vec3<decltype(a.y*b.z)>
    {
        return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x,
        };
    }

}//ns