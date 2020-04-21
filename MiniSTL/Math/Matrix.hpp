#pragma once
#include "MiniSTL/Types.hpp"
#include <cmath>

namespace mini::math
{
    //used for vector access mat[x0] == mat [0][0]
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
        Mat<decltype(a[0][0] + b[0][0]), Y, X> mat;

        for (u8 y = 0; y < Y; ++y) {
        for (u8 x = 0; x < X; ++x) {
            mat[y][x] = a[y][x] + b[y][x];
        }}

        return mat;
    }

    template<class T1, class T2, u8 Y, u8 X> 
    void operator+=(Mat<T1, Y, X>& a, const Mat<T2, Y, X>& b)
    {
        for (u8 y = 0; y < Y; ++y) {
        for (u8 x = 0; x < X; ++x) {
            a[y][x] += b[y][x];
        }}
    }

    ///subtraction

    template<class T1, class T2, u8 Y, u8 X> [[nodiscard]]
    auto operator-(const Mat<T1, Y, X>& a, const Mat<T2, Y, X>& b)
    {
        Mat<decltype(a[0][0] - b[0][0]), Y, X> mat;

        for (u8 y = 0; y < Y; ++y) {
        for (u8 x = 0; x < X; ++x) {
            mat[y][x] = a[y][x] - b[y][x];
        }}

        return mat;
    }

    template<class T1, class T2, u8 Y, u8 X> 
    void operator-=(Mat<T1, Y, X>& a, const Mat<T2, Y, X>& b)
    {
        for (u8 y = 0; y < Y; ++y) {
        for (u8 x = 0; x < X; ++x) {
            a[y][x] -= b[y][x];
        }}
    }

    ///scalar multiplication

    template<class T1, class T2, u8 Y, u8 X> [[nodiscard]]
    auto operator*(const Mat<T1, Y, X>& a, const T2& scalar)
    {
        Mat<decltype(a[0][0] * scalar), Y, X> mat;

        for (u8 y = 0; y < Y; ++y) {
        for (u8 x = 0; x < X; ++x) {
            mat[y][x] = a[y][x] * scalar;
        }}

        return mat;
    }

    template<class T1, class T2, u8 Y, u8 X> 
    void operator*=(Mat<T1, Y, X>& a, const T2& scalar)
    {
        for (u8 y = 0; y < Y; ++y) {
        for (u8 x = 0; x < X; ++x) {
            a[y][x] *= scalar;
        }}
    }

    ///scalar division

    template<class T1, class T2, u8 Y, u8 X> [[nodiscard]]
    auto operator/(const Mat<T1, Y, X>& a, const T2& scalar)
    {
        Mat<decltype(a[0][0] / scalar), Y, X> mat;

        for (u8 y = 0; y < Y; ++y) {
        for (u8 x = 0; x < X; ++x) {
            mat[y][x] = a[y][x] / scalar;
        }}

        return mat;
    }

    template<class T1, class T2, u8 Y, u8 X> 
    void operator/=(Mat<T1, Y, X>& a, const T2& scalar)
    {
        for (u8 y = 0; y < Y; ++y) {
        for (u8 x = 0; x < X; ++x) {
            a[y][x] /= scalar;
        }}
    }

    ///vector magnitude

    template<class T, u8 X, typename F = f32>
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

    template<class T, u8 X, typename F = f32>
    auto Normalize(const Vec<T, X>& vec)
    {
        const F mag = Magnitude(vec);

        Vec<F, X> out;
        for (u8 x = 0; x < X; ++x) {
            out[0][x] = vec[0][x] / mag;
        }

        return out;
    }

    template<class T, u8 X, typename F = f32>
    void NormalizeThis(Vec<T, X>& vec)
    {
        const auto mag = Magnitude(vec);
        for (u8 x = 0; x < X; ++x) {
            vec[0][x] /= mag;
        }
    }



    ///vector dot product



    //return a.x * b.x + a.y * b.y + a.z * b.z;


    ///vector cross product

    //return {
        //        a.y * b.z - a.z * b.y,
        //        a.z * b.x - a.x * b.z,
        //        a.x * b.y - a.y * b.x,
        //    };


    //template<class M, class V>
    //auto operator*(const Mat4<M> mat, const Vec4<V>& vec) -> Vec4<decltype(mat[0][0]*vec.x)>
    //{
    //    return {
    //        //mat[0][0] * vec.x
    //    
    //    };
    //}

    //template<class M, class V>
    //auto operator*(const Vec4<V>& vec, const Mat4<M> mat) -> Vec4<decltype(mat[0][0] * vec.x)>
    //{
    //    return {
    //
    //
    //    };
    //}

    ///normalize

    ///project

}