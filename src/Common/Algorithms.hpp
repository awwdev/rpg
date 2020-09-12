#pragma once

#include "Common/Structs.hpp"
#include "Common/Container/Optional.hpp"

namespace rpg::use {
    
template<typename VAL1, typename VAL2>
auto Max(const VAL1& val1, const VAL2& val2)
{
    return (val1 > val2) ? val1 : val2;
}

template<class POINT, class RECT, 
    typename = IsArithmetic<POINT>, 
    typename = IsArithmetic<RECT>>
bool IsPointInsideRect(const POINT x, const POINT y, const use::Rect<RECT>& rect)
{
    return (x > rect.x && x < rect.x + rect.w &&
            y > rect.y && y < rect.y + rect.h);
}

template<auto MIN, auto MAX, typename T>
void Clamp(T& val)
{
    if (val > MAX) {
        val = MAX;
    }
    else if (val < MIN) {
        val = MIN;
    }
}

template<typename MIN, typename MAX, typename T>
void Clamp(T& val, const MIN min, const MAX max)
{
    if (val > max) {
        val = max;
    }
    else if (val < min) {
        val = min;
    }
}

template<typename MIN, typename MAX, typename T>
bool ClampReturnBool(T& val, const MIN min, const MAX max)
{
    if (val > max) {
        val = max;
        return true;
    }
    else if (val < min) {
        val = min;
        return true;
    }
    return false;
}

struct Intersection
{
    use::Vec3f pos;
    float u, v;

    //TODO: write better
    idx_t GetClosestVertex(const idx_t vIdx) const
    {
        if      (u > 0.5) return vIdx + 1;
        else if (v > 0.5) return vIdx + 2;
        else if (u > 0.25 && v > 0.25)
        {
            if  (u > v) return vIdx + 1;
            else        return vIdx + 2;
        }
        return vIdx;
    }
};

com::Optional<Intersection> RayTriangleIntersection(
    const use::Vec3f& rayOrigin, 
    const use::Vec3f& rayDir, 
    const use::Vec3f& v0,
    const use::Vec3f& v1,
    const use::Vec3f& v2)
{
    using namespace use;
    constexpr float EPSILON = 0.0000001f;

    const auto edge1 = v1 - v0;
    const auto edge2 = v2 - v0;

    const auto h = Cross(rayDir, edge2);
    const auto a = Dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return {};

    const auto f = 1.f / a;
    const auto s = (rayOrigin*-1.f) - v0;
    const auto u = f * Dot(s, h);
    if (u < 0.0 || u > 1.0)
        return {};

    const auto q = Cross(s, edge1);
    const auto v = f * Dot(rayDir, q);
    if (v < 0.0 || u + v > 1.0)
        return {};

    const float t = f * Dot(edge2, q);
    if (t > EPSILON) 
        return Intersection{ (rayOrigin*-1.f) + rayDir * t, u, v };

    return {};
}

template<class T>
constexpr T Sin(const T val)
{
    return T{2}*val / (T{1} + val*val);
}

template<class T>
constexpr T Cos(const T val)
{
    return (T{1} - val*val) / (T{1} + val*val);
}

//? EASING

template<class T>
constexpr T Ease(const T val, const T p = 2)
{
    return (T)std::pow(val, p);
}

//? STRLEN

idx_t StrLen(chars_t str)
{
    idx_t i = 0;
    while(str[i] != '\0') ++i;
    return i;
}

}//ns