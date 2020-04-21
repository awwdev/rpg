#pragma once
#include "MiniSTL/Types.hpp"
#include <cmath>
#include <cstring>

namespace mini::math
{
    //used for vector access mat[X0] == mat [0][0]
    enum VX : u8 { X0 = 0, X1 = 1, X2 = 2, X3 = 3 };


    template<class T, u8 Y, u8 X, typename = IsArithmetic<T>>
    struct Mat
    {
        T cells[Y][X];

        //matrix access [][] (no need to .cells[][])
        inline T*       operator[](const u8 i)       { return cells[i]; }
        inline const T* operator[](const u8 i) const { return cells[i]; } 

        //vector access
        inline T&       operator[](const VX x)       { return cells[0][x]; }
        inline const T& operator[](const VX x) const { return cells[0][x]; } 
    };


    template<class T, u8 X>
    using Vec = Mat<T, 1, X>;

    using Mat3f = Mat<f32, 4, 4>;
    using Mat4f = Mat<f32, 3, 3>;
    using Vec2f = Vec<f32, 2>;
    using Vec3f = Vec<f32, 3>;
    using Vec4f = Vec<f32, 4>;

    ///comparsion

    template<class T, u8 Y, u8 X>
    bool operator==(const Mat<T, Y, X>& a, const Mat<T, Y, X>& b)
    {
        const auto cmp = std::memcmp(a.cells, b.cells, Y*X*sizeof(T));
        return cmp == 0;
    }

    ///identity

    template<class T = f32> [[nodiscard]]
    auto Identity4x4() -> Mat<T, 4, 4>
    {
        return {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        };
    }

    template<class T = float> [[nodiscard]]
    auto Identity3x3() -> Mat<T, 3, 3>
    {
        return {
            1, 0, 0,
            0, 1, 0,
            0, 0, 1,
        };
    }

    ///addition

    template<class T1, class T2, u8 Y, u8 X> [[nodiscard]]
    auto operator+(const Mat<T1, Y, X>& a, const Mat<T2, Y, X>& b)
    {
        Mat<decltype(a[0][0] + b[0][0]), Y, X> out;

        for (u8 y = 0; y < Y; ++y) {
        for (u8 x = 0; x < X; ++x) {
            out[y][x] = a[y][x] + b[y][x];
        }}

        return out;
    }

    template<class T1, class T2, u8 Y, u8 X> 
    void operator+=(Mat<T1, Y, X>& ref, const Mat<T2, Y, X>& other)
    {
        for (u8 y = 0; y < Y; ++y) {
        for (u8 x = 0; x < X; ++x) {
            ref[y][x] += other[y][x];
        }}
    }

    ///subtraction

    template<class T1, class T2, u8 Y, u8 X> [[nodiscard]]
    auto operator-(const Mat<T1, Y, X>& a, const Mat<T2, Y, X>& b) //not commutative
    {
        Mat<decltype(a[0][0] - b[0][0]), Y, X> out;

        for (u8 y = 0; y < Y; ++y) {
        for (u8 x = 0; x < X; ++x) {
            out[y][x] = a[y][x] - b[y][x];
        }}

        return out;
    }

    template<class T1, class T2, u8 Y, u8 X> 
    void operator-=(Mat<T1, Y, X>& ref, const Mat<T2, Y, X>& other)
    {
        for (u8 y = 0; y < Y; ++y) {
        for (u8 x = 0; x < X; ++x) {
            ref[y][x] -= other[y][x];
        }}
    }

    ///scalar multiplication

    template<class T, class S, u8 Y, u8 X, typename = IsArithmetic<S>> [[nodiscard]]
    auto operator*(const Mat<T, Y, X>& m, const S& scalar)
    {
        Mat<decltype(m[0][0] * scalar), Y, X> out;

        for (u8 y = 0; y < Y; ++y) {
        for (u8 x = 0; x < X; ++x) {
            out[y][x] = m[y][x] * scalar;
        }}

        return out;
    }

    template<class T1, class S, u8 Y, u8 X, typename = IsArithmetic<S>>
    void operator*=(Mat<T1, Y, X>& ref, const S& scalar)
    {
        for (u8 y = 0; y < Y; ++y) {
        for (u8 x = 0; x < X; ++x) {
            ref[y][x] *= scalar;
        }}
    }

    ///scalar division

    template<class T, class S, u8 Y, u8 X, typename = IsArithmetic<S>> [[nodiscard]]
    auto operator/(const Mat<T, Y, X>& m, const S& scalar)
    {
        Mat<decltype(m[0][0] / scalar), Y, X> out;

        for (u8 y = 0; y < Y; ++y) {
        for (u8 x = 0; x < X; ++x) {
            out[y][x] = m[y][x] / scalar;
        }}

        return out;
    }

    template<class T, class S, u8 Y, u8 X, typename = IsArithmetic<S>>
    void operator/=(Mat<T, Y, X>& ref, const S& scalar)
    {
        for (u8 y = 0; y < Y; ++y) {
        for (u8 x = 0; x < X; ++x) {
            ref[y][x] /= scalar;
        }}
    }

    ///vector magnitude

    template<class T, u8 X, typename F = f32> [[nodiscard]]
    F Magnitude(const Vec<T, X>& vec)
    {
        T mag = 0;
        for (u8 x = 0; x < X; ++x) {
            mag += vec[0][x] * vec[0][x];
        }

        //todo: faster square root method (like quake sqrt)
        if constexpr (std::is_same_v<F, f32>) return std::sqrtf(mag);
        if constexpr (std::is_same_v<F, f64>) return std::sqrt(mag);
    }

    ///vector normalize

    template<class T, u8 X, typename F = f32> [[nodiscard]]
    auto Normalize(const Vec<T, X>& vec)
    {
        const F mag = Magnitude(vec);

        Vec<F, X> out;
        for (u8 x = 0; x < X; ++x) {
            out[0][x] = vec[0][x] / mag;
        }

        return out;
    }

    template<class T, u8 X>
    void NormalizeThis(Vec<T, X>& ref)
    {
        const auto mag = Magnitude(ref);
        for (u8 x = 0; x < X; ++x) {
            ref[0][x] /= mag;
        }
    }

    ///vector dot product

    template<class T1, class T2, u8 X> [[nodiscard]]
    auto Dot(const Vec<T1, X>& v1, const Vec<T2, X>& v2)
    {
        decltype(v1[0][0] * v2[0][0]) out = 0; //scalar

        for (u8 x = 0; x < X; ++x)
        {
            out += v1[0][x] * v2[0][x];
        }

        return out;
    }

    ///vector cross product

    template<class T1, class T2> [[nodiscard]]
    auto Cross(const Vec<T1, 3>& v1, const Vec<T2, 3>& v2) //not commutative
    {
        //does one ever need 2d cross product?
        return Vec<decltype(v1[0][0] * v2[0][0]), 3>
        {
            v1[0][1] * v2[0][2] - v1[0][2] * v2[0][1],
            v1[0][2] * v2[0][0] - v1[0][0] * v2[0][2],
            v1[0][0] * v2[0][1] - v1[0][1] * v2[0][0],
        };
    }

    ///multiplication 

    template<class T1, class T2, u8 Y1, u8 X1, u8 X2> [[nodiscard]]
    auto operator*(const Mat<T1, Y1, X1>& a, const Mat<T2, X1, X2>& b) //requirement: columns1 == rows2
    {
        Mat<decltype(a[0][0] * b[0][0]), Y1, X2> out {};

        for (u8 y = 0; y < Y1; ++y) {
        for (u8 x = 0; x < X2; ++x) {

            for (u8 c = 0; c < X1; ++c)
                out[y][x] += a[y][c] * b[c][x];
            
        }}

        return out;
    }


    ///rotation matrix



    ///transformation

    void Translate()
    {

    }

    void Scale()
    {

    }

    void Rotate()
    {

    }


    ///projection

}