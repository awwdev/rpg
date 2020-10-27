//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include <cmath>
#include <cstring>
#include <iostream>
#include <ios>
#include <iomanip>

//! quaternion calculations have dedicated methods, do not use e.g. operator* 

namespace rpg::com {


constexpr float PI  = 3.14159265359f;
constexpr float PIH = 1/180.f;

template<typename T, auto Y_T, auto X_T>
struct Mat
{
    static constexpr auto Y = Y_T;
    static constexpr auto X = X_T;

    union
    {
        T data [Y][X];

        //vector access (out of bounds is UB)
        //using those while the array is active (is always teh case) is technically UB
        struct {
            T x, y; 
            union {
                //! w is not width (this can lead to confusion)
                struct { T z, w; };
                struct { T width, height; };
            };
        };
        struct { T r, g, b, a; };
        
    };

    //allow mat[][] access
    T*       operator[](const u8 y)       { return data[y]; } 
    const T* operator[](const u8 y) const { return data[y]; } 
};

using Mat4f = Mat<float, 4, 4>;

template<typename T, auto X>
using Vec = Mat<T, 1, X>;
template<typename T>
using Vec3 = Mat<T, 1, 3>;
template<typename T>
using Vec4 = Mat<T, 1, 4>;

using Vec2u = Vec<u32, 2>;
using Vec2i = Vec<s32, 2>;
using Vec4f = Vec<float, 4>;
using Vec3f = Vec<float, 3>;
using Vec2f = Vec<float, 2>;
using Quatf = Vec4f;
using Rectf = Vec4f;

inline Mat4f Identity4()
{
    return {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
}

template<typename T, typename S>
inline T InitializeWith(S&& value)
{
    T mat;
    for(auto y = 0; y < T::Y; ++y) {
    for(auto x = 0; x < T::X; ++x) {
        mat[y][x] = value;
    }}
    return mat;
}

//? VECTOR

inline Vec3f TruncateVec4(const Vec4f& vec)
{
    return { vec.x, vec.y, vec.z };
}

inline Vec4f MakeHomoVec(const Vec3f& vec)
{
    return { vec.x, vec.y, vec.z, 1 };
}

inline float FastSqrt(float number)
{
    //from quake <3 
    //TODO: check input?
    float x2 = number * 0.5f;
    float y  = number;

    std::uint32_t i;
    std::memcpy(&i, &y, sizeof(float));
    i = 0x5f3759df - (i >> 1);
    std::memcpy(&y, &i, sizeof(float));

    return number * (y * (1.5f - (x2 * y * y)));
}

template<typename T, auto X>
float Magnitude(const Vec<T, X>& vec)
{
    T sum = 0;
    for (u8 x = 0; x < X; ++x) { sum += vec[0][x] * vec[0][x]; }
    return FastSqrt(sum);
}

template<typename T, auto X>
auto Normalize(const Vec<T, X>& vec)
{
    Vec<T, X> out {};
    
    const auto mag = Magnitude(vec);
    if (mag == 0 || std::isnan(mag)) 
        return out;
        
    for (u8 x = 0; x < X; ++x) { out[0][x] = vec[0][x] / mag; }
    return out;
}

template<typename T, auto X>
void NormalizeThis(Vec<T, X>& vec)
{
    const auto mag = Magnitude(vec);
    if (mag == 0 || std::isnan(mag)) 
        return;

    for (u8 x = 0; x < X; ++x) { vec[0][x] /= mag; }
}

template<typename T, auto X>
float Dot(const Vec<T, X>& v1, const Vec<T, X>& v2)
{
    float out = 0;
    for (u8 x = 0; x < X; ++x) { out += v1[0][x] * v2[0][x]; }
    return out;
}

template<typename T>
auto Cross(const Vec<T, 3>& v1, const Vec<T, 3>& v2)
{
    return Vec<T, 3> {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
    };
}

//will reset w to 1 !!!
template<typename T>
auto Cross(const Vec<T, 4>& v1, const Vec<T, 4>& v2)
{
    return Vec<T, 4> {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
        1
    };
}

template<typename T>
auto Distance(const Vec<T, 3>& v2, const Vec<T, 3>& v1)
{
    return Magnitude(v2 - v1);
}

//? MAT MULT

template<typename T, auto Y1, auto X1, auto X2>
auto operator*(
    const Mat<T, X1, X2>& m1, 
    const Mat<T, Y1, X1>& m2)
{
    Mat<T, Y1, X2> out {};

    for (u8 y = 0; y < Y1; ++y) {
    for (u8 x = 0; x < X2; ++x) {

        for (u8 c = 0; c < X1; ++c)
            out[y][x] += m2[y][c] * m1[c][x];
    }}
    return out;
}

template<typename T1, class T2, auto Y, auto X>
auto operator*(const Mat<T1, Y, X>& mat, const T2 scalar)
{
    Mat<T1, Y, X> out { mat };
    for (u8 y = 0; y < Y; ++y) {
    for (u8 x = 0; x < X; ++x) {
        out[y][x] *=  scalar;
    }}
    return out;
}

template<typename T1, class T2, auto Y, auto X>
auto operator/(const Mat<T1, Y, X>& mat, const T2 scalar)
{
    Mat<T1, Y, X> out { mat };
    for (u8 y = 0; y < Y; ++y) {
    for (u8 x = 0; x < X; ++x) {
        out[y][x] /=  scalar;
    }}
    return out;
}

template<typename T1, auto Y, auto X>
auto operator/(as_arithmetic auto const scalar, const Mat<T1, Y, X>& mat)
{
    Mat<T1, Y, X> out { mat };
    for (u8 y = 0; y < Y; ++y) {
    for (u8 x = 0; x < X; ++x) {
        out[y][x] =  scalar / out[y][x];
    }}
    return out;
}

template<typename T, auto Y, auto X>
auto operator+(const Mat<T, Y, X>& m1, const Mat<T, Y, X>& m2) 
{
    Mat<T, Y, X> out;
    for (u8 y = 0; y < Y; ++y) {
    for (u8 x = 0; x < X; ++x) {
        out[y][x] = m1[y][x] + m2[y][x];
    }}
    return out;
}

template<typename T, auto Y, auto X>
auto operator-(const Mat<T, Y, X>& m1, const Mat<T, Y, X>& m2) 
{
    Mat<T, Y, X> out;
    for (u8 y = 0; y < Y; ++y) {
    for (u8 x = 0; x < X; ++x) {
        out[y][x] = m1[y][x] - m2[y][x];
    }}
    return out;
}

template<typename T, auto Y, auto X, class SCALAR>
auto operator-(const Mat<T, Y, X>& m1, const SCALAR scalar) 
{
    Mat<T, Y, X> out;
    for (u8 y = 0; y < Y; ++y) {
    for (u8 x = 0; x < X; ++x) {
        out[y][x] = m1[y][x] - scalar;
    }}
    return out;
}

//? MAT INVERSE

template<typename T, auto Y, auto X>
inline auto Inverse(const Mat<T, Y, X>& m)
{
    //from https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix

    auto A2323 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
    auto A1323 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
    auto A1223 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
    auto A0323 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
    auto A0223 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
    auto A0123 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
    auto A2313 = m[1][2] * m[3][3] - m[1][3] * m[3][2];
    auto A1313 = m[1][1] * m[3][3] - m[1][3] * m[3][1];
    auto A1213 = m[1][1] * m[3][2] - m[1][2] * m[3][1];
    auto A2312 = m[1][2] * m[2][3] - m[1][3] * m[2][2];
    auto A1312 = m[1][1] * m[2][3] - m[1][3] * m[2][1];
    auto A1212 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
    auto A0313 = m[1][0] * m[3][3] - m[1][3] * m[3][0];
    auto A0213 = m[1][0] * m[3][2] - m[1][2] * m[3][0];
    auto A0312 = m[1][0] * m[2][3] - m[1][3] * m[2][0];
    auto A0212 = m[1][0] * m[2][2] - m[1][2] * m[2][0];
    auto A0113 = m[1][0] * m[3][1] - m[1][1] * m[3][0];
    auto A0112 = m[1][0] * m[2][1] - m[1][1] * m[2][0];

    auto det = m[0][0] * ( m[1][1] * A2323 - m[1][2] * A1323 + m[1][3] * A1223 ) 
        - m[0][1] * ( m[1][0] * A2323 - m[1][2] * A0323 + m[1][3] * A0223 ) 
        + m[0][2] * ( m[1][0] * A1323 - m[1][1] * A0323 + m[1][3] * A0123 ) 
        - m[0][3] * ( m[1][0] * A1223 - m[1][1] * A0223 + m[1][2] * A0123 ) ;
    det = 1 / det;

    return Mat<T, Y, X> {
        det *   ( m[1][1] * A2323 - m[1][2] * A1323 + m[1][3] * A1223 ),
        det * - ( m[0][1] * A2323 - m[0][2] * A1323 + m[0][3] * A1223 ),
        det *   ( m[0][1] * A2313 - m[0][2] * A1313 + m[0][3] * A1213 ),
        det * - ( m[0][1] * A2312 - m[0][2] * A1312 + m[0][3] * A1212 ),
        det * - ( m[1][0] * A2323 - m[1][2] * A0323 + m[1][3] * A0223 ),
        det *   ( m[0][0] * A2323 - m[0][2] * A0323 + m[0][3] * A0223 ),
        det * - ( m[0][0] * A2313 - m[0][2] * A0313 + m[0][3] * A0213 ),
        det *   ( m[0][0] * A2312 - m[0][2] * A0312 + m[0][3] * A0212 ),
        det *   ( m[1][0] * A1323 - m[1][1] * A0323 + m[1][3] * A0123 ),
        det * - ( m[0][0] * A1323 - m[0][1] * A0323 + m[0][3] * A0123 ),
        det *   ( m[0][0] * A1313 - m[0][1] * A0313 + m[0][3] * A0113 ),
        det * - ( m[0][0] * A1312 - m[0][1] * A0312 + m[0][3] * A0112 ),
        det * - ( m[1][0] * A1223 - m[1][1] * A0223 + m[1][2] * A0123 ),
        det *   ( m[0][0] * A1223 - m[0][1] * A0223 + m[0][2] * A0123 ),
        det * - ( m[0][0] * A1213 - m[0][1] * A0213 + m[0][2] * A0113 ),
        det *   ( m[0][0] * A1212 - m[0][1] * A0212 + m[0][2] * A0112 ),
    };
}

//? ROTATION

inline Mat4f MatAngleAxisX(const f32 degree)
{
    const auto rad = degree * PI * PIH;
    const auto c = (f32)cosf(rad);
    const auto s = (f32)sinf(rad);
    return {
        1, 0, 0, 0,
        0, c,-s, 0,
        0, s, c, 0,
        0, 0, 0, 1,
    };
}

inline Mat4f MatAngleAxisY(const f32 degree)
{
    const auto rad = degree * PI * PIH;
    const auto c = (f32)cosf(rad);
    const auto s = (f32)sinf(rad);
    return {
        c, 0, s, 0,
        0, 1, 0, 0,
        -s, 0, c, 0,
        0, 0, 0, 1,
    };
}

inline Mat4f MatAngleAxisZ(const f32 degree)
{
    const auto rad = degree * PI * PIH;
    const auto c = (f32)cosf(rad);
    const auto s = (f32)sinf(rad);
    return {
        c,-s, 0, 0,
        s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
}

//? QUATERNION

inline Quatf QuatAngleAxis(const float degree, const Vec3f& unitAxis)
{
    const float radh = degree * PI * PIH * 0.5f;
    const float s = (f32)sinf(radh);
    return {
        unitAxis.x * s,
        unitAxis.y * s,
        unitAxis.z * s,
        (f32)cosf(radh),
    };
}

inline Vec3f QuatMultVec(const Quatf& q, const Vec3f& v)
{
    const Vec3f u { q.x, q.y, q.z };
    const float s { q.w };
    return { u * Dot(u, v) * 2.0f + v * (s*s - Dot(u, u)) + Cross(u, v) * s * 2.0f };
}

inline Quatf QuatMultQuat(const Quatf& q1, const Quatf& q2)
{
    return {
         q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x,
        -q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y,
         q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z,
        -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w,
    };
}

inline Mat4f QuatToMat(const Quatf& q)
{
    //from StackOverflow (forgot the link, sorry!)
    const float y2 = 2*q.y*q.y;
    const float z2 = 2*q.z*q.z;
    const float x2 = 2*q.x*q.x;
                    
    const float xw = 2*q.x*q.w;
    const float xy = 2*q.x*q.y;
    const float xz = 2*q.x*q.z;
    const float yw = 2*q.y*q.w;
    const float yz = 2*q.y*q.z;
    const float zw = 2*q.z*q.w;

    const float a =  1 - y2 - z2;
    const float b = xy - zw;
    const float c = xz + yw;
    const float d = xy + zw;
    const float e = 1 - x2 - z2;
    const float f = yz - xw;
    const float g = xz - yw;
    const float h = yz + xw;
    const float i = 1 - x2 - y2;

    return {
        a, b, c, 0, 
        d, e, f, 0, 
        g, h, i, 0, 
        0, 0, 0, 1
    };
}

//? HELPER

inline Mat4f LookAt(const Vec3f& eye, const Vec3f& at)
{
    //mostly from GLM
    const Vec3f f = Normalize(at - eye);
    const Vec3f r = Normalize(Cross(f, Vec3f{ 0, 1, 0 }));
    const Vec3f u = Cross(r, f);
    const Vec3f e = { Dot(r, eye), Dot(u, eye), Dot(f, eye)};

    return { 
        r.x, u.x,-f.x, 0,
        r.y, u.y,-f.y, 0,
        r.z, u.z,-f.z, 0,
        -e.x,-e.y, e.z, 1
    };
}

//? PRINT

template<typename T, auto Y, auto X>
void PrintMatrix(const Mat<T, Y, X>& mat, const u8 precision = 4, const u8 width = 8)
{
    const std::ios_base::fmtflags coutFlags (std::cout.flags());
    std::cout << std::fixed << std::setprecision(precision) << std::right;
    for(auto y = 0; y < Y; ++y) {
    for(auto x = 0; x < X; ++x) {
        std::cout << std::setw(width) << mat[y][x] << ' ';
    }
    std::cout << '\n';
    }
    std::cout.flags(coutFlags); //reset
}

}//ns