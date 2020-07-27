#pragma once

#include "mini/Utils/Structs.hpp"
#include "mini/Box/Optional.hpp"

namespace mini::utils
{
    template<typename VAL1, typename VAL2>
    bool Max(const VAL1& val1, const VAL2& val2)
    {
        return (val1 > val2) ? val1 : val2;
    }

    template<class POINT, class RECT, 
        typename = IsArithmetic<POINT>, 
        typename = IsArithmetic<RECT>>
    bool IsPointInsideRect(const POINT x, const POINT y, const utils::Rect<RECT>& rect)
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

    struct Intersection
    {
        math::Vec3f pos;
        float u, v;
    };

    box::Optional<Intersection> RayTriangleIntersection(
        const math::Vec3f& rayOrigin, 
        const math::Vec3f& rayDir, 
        const math::Vec3f& v0,
        const math::Vec3f& v1,
        const math::Vec3f& v2)
    {
        using namespace math;
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

}//ns