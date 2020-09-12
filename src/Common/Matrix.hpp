//https://github.com/awwdev

#pragma once
#include "Common/Types.hpp"
#include <cmath>
#include <cstring>
#include <iostream>
#include <ios>
#include <iomanip>

//! quaternion calculations have dedicated methods, do not use e.g. operator* 

namespace rpg::use {

enum V { X, Y, Z, W }; //vector access of Mat, e.g. vec[X]
constexpr float PI  = 3.14159265359f;
constexpr float PIH = 1/180.f;

template<class T, auto Y, auto X>
struct Mat
{
    T data[Y][X];
    //TODO making additional union (even if it would be UB)

    T*       operator[](const u8 y)       { return data[y]; } 
    const T* operator[](const u8 y) const { return data[y]; } 
    T&       operator[](const V  x)       { return data[0][x]; }
    const T& operator[](const V  x) const { return data[0][x]; }
};

using Mat4f = Mat<float, 4, 4>;

template<class T, auto X>
using Vec   = Mat<T, 1, X>;
using Vec2u = Vec<u32, 2>;
using Vec4f = Vec<float, 4>;
using Vec3f = Vec<float, 3>;
using Vec2f = Vec<float, 2>;
using Quatf = Vec4f;

Mat4f Identity4()
{
    return {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
}

//? VECTOR

inline Vec3f TruncateVec4(const Vec4f& vec)
{
    return { vec[X], vec[Y], vec[Z] };
}

inline Vec4f MakeHomoVec(const Vec3f& vec)
{
    return { vec[X], vec[Y], vec[Z], 1 };
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

template<class T, auto X>
float Magnitude(const Vec<T, X>& vec)
{
    T sum = 0;
    for (u8 x = 0; x < X; ++x) { sum += vec[0][x] * vec[0][x]; }
    return FastSqrt(sum);
}

template<class T, auto X>
auto Normalize(const Vec<T, X>& vec)
{
    Vec<T, X> out {};
    
    const auto mag = Magnitude(vec);
    if (mag == 0 || std::isnan(mag)) 
        return out;
        
    for (u8 x = 0; x < X; ++x) { out[0][x] = vec[0][x] / mag; }
    return out;
}

template<class T, auto X>
void NormalizeThis(Vec<T, X>& vec)
{
    const auto mag = Magnitude(vec);
    if (mag == 0 || std::isnan(mag)) 
        return;

    for (u8 x = 0; x < X; ++x) { vec[0][x] /= mag; }
}

template<class T, auto X>
float Dot(const Vec<T, X>& v1, const Vec<T, X>& v2)
{
    float out = 0;
    for (u8 x = 0; x < X; ++x) { out += v1[0][x] * v2[0][x]; }
    return out;
}

template<class T>
auto Cross(const Vec<T, 3>& v1, const Vec<T, 3>& v2)
{
    return Vec<T, 3> {
        v1[Y] * v2[Z] - v1[Z] * v2[Y],
        v1[Z] * v2[X] - v1[X] * v2[Z],
        v1[X] * v2[Y] - v1[Y] * v2[X],
    };
}

//! this is not a true cross product, it resets w to 1
template<class T>
auto Cross(const Vec<T, 4>& v1, const Vec<T, 4>& v2)
{
    return Vec<T, 4> {
        v1[Y] * v2[Z] - v1[Z] * v2[Y],
        v1[Z] * v2[X] - v1[X] * v2[Z],
        v1[X] * v2[Y] - v1[Y] * v2[X],
        1
    };
}

template<class T>
auto Distance(const Vec<T, 3>& v2, const Vec<T, 3>& v1)
{
    return Magnitude(v2 - v1);
}

//? MAT MULT

template<class T, auto Y1, auto X1, auto X2>
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

template<class T1, class T2, auto Y, auto X>
auto operator*(const Mat<T1, Y, X>& mat, const T2 scalar)
{
    Mat<T1, Y, X> out { mat };
    for (u8 y = 0; y < Y; ++y) {
    for (u8 x = 0; x < X; ++x) {
        out[y][x] *=  scalar;
    }}
    return out;
}

template<class T1, class T2, auto Y, auto X>
auto operator/(const Mat<T1, Y, X>& mat, const T2 scalar)
{
    Mat<T1, Y, X> out { mat };
    for (u8 y = 0; y < Y; ++y) {
    for (u8 x = 0; x < X; ++x) {
        out[y][x] /=  scalar;
    }}
    return out;
}

template<class T, auto Y, auto X>
auto operator+(const Mat<T, Y, X>& m1, const Mat<T, Y, X>& m2) 
{
    Mat<T, Y, X> out;
    for (u8 y = 0; y < Y; ++y) {
    for (u8 x = 0; x < X; ++x) {
        out[y][x] = m1[y][x] + m2[y][x];
    }}
    return out;
}

template<class T, auto Y, auto X>
auto operator-(const Mat<T, Y, X>& m1, const Mat<T, Y, X>& m2) 
{
    Mat<T, Y, X> out;
    for (u8 y = 0; y < Y; ++y) {
    for (u8 x = 0; x < X; ++x) {
        out[y][x] = m1[y][x] - m2[y][x];
    }}
    return out;
}

//? MAT INVERSE

template<class T, auto Y, auto X>
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
        unitAxis[X] * s,
        unitAxis[Y] * s,
        unitAxis[Z] * s,
        (f32)cosf(radh),
    };
}

inline Vec3f QuatMultVec(const Quatf& q, const Vec3f& v)
{
    const Vec3f u { q[X], q[Y], q[Z] };
    const float s { q[W] };
    return { u * Dot(u, v) * 2.0f + v * (s*s - Dot(u, u)) + Cross(u, v) * s * 2.0f };
}

inline Quatf QuatMultQuat(const Quatf& q1, const Quatf& q2)
{
    return {
         q1[X] * q2[W] + q1[Y] * q2[Z] - q1[Z] * q2[Y] + q1[W] * q2[X],
        -q1[X] * q2[Z] + q1[Y] * q2[W] + q1[Z] * q2[X] + q1[W] * q2[Y],
         q1[X] * q2[Y] - q1[Y] * q2[X] + q1[Z] * q2[W] + q1[W] * q2[Z],
        -q1[X] * q2[X] - q1[Y] * q2[Y] - q1[Z] * q2[Z] + q1[W] * q2[W],
    };
}

inline Mat4f QuatToMat(const Quatf& q)
{
    //from StackOverflow (forgot the link, sorry!)
    const float y2 = 2*q[Y]*q[Y];
    const float z2 = 2*q[Z]*q[Z];
    const float x2 = 2*q[X]*q[X];
                    
    const float xw = 2*q[X]*q[W];
    const float xy = 2*q[X]*q[Y];
    const float xz = 2*q[X]*q[Z];
    const float yw = 2*q[Y]*q[W];
    const float yz = 2*q[Y]*q[Z];
    const float zw = 2*q[Z]*q[W];

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
        r[X], u[X],-f[X], 0,
        r[Y], u[Y],-f[Y], 0,
        r[Z], u[Z],-f[Z], 0,
        -e[X],-e[Y], e[Z], 1
    };
}

//? PRINT

template<class T, auto Y, auto X>
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