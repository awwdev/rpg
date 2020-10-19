//https://github.com/awwdev

#pragma once
#include "com/Types.hpp"
#include "com/Matrix.hpp"
#include "dbg/Assert.hpp"

namespace rpg::com {
    
//? clamping

inline constexpr
auto Max(auto const val1, auto const val2)
{
    return (val1 > val2) ? val1 : val2;
}

inline constexpr
auto Min(auto const val1, auto const val2)
{
    return (val1 < val2) ? val1 : val2;
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

//? trigonometry

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

//? easing

template<class T>
constexpr T Ease(const T val, const T p = 2)
{
    return (T)std::pow(val, p);
}

//? strlen

inline auto StrLen(chars_t str)
{
    idx_t len = 0;
    while(str[len] != '\0') ++len;
    return len;
}

//? serialization

template<typename T>
void WriteBinaryFile_C_Array(chars_t path, T const* ptr, idx_t const count)
{
    auto file = std::ofstream(path, std::ios::binary);
    dbg::Assert(file.is_open(), "[IO] cannot open file");
    file.write(reinterpret_cast<char const*>(ptr), count * sizeof(T));
}

template<typename T>
void ReadBinaryFile_C_Array(chars_t path, T* ptr)
{
    auto file = std::ifstream(path, std::ios::binary | std::ios::ate);
    dbg::Assert(file.is_open(), "[IO] cannot open file");
    auto size = file.tellg();
    file.seekg(std::ios::beg);
    file.read(reinterpret_cast<char*>(ptr), size);
}

//? printing

template<typename T, auto Y, auto X>
void Print2DArray(T const (&arr)[Y][X])
{
    for(idx_t y = 0; y < Y; ++y) {
    for(idx_t x = 0; x < X; ++x) {
        std::cout << arr[y][x] << '\n';
    }}
}

template<typename T, auto N>
void Print1DArray(T const (&arr)[N])
{
    for(idx_t i = 0; i < N; ++i) {
        std::cout << arr[i] << '\n';
    }
}

//? scroll / zap through enum

inline auto ScrollEnum(as_scrollable_enum auto pEnum)
{
    using T = decltype(pEnum);
    using U = std::underlying_type_t<T>;
    return static_cast<T>(
        (static_cast<U>(pEnum) + 1) % 
         static_cast<U>(T::ENUM_END)
    );
}

//? axis-aligned bounding box

struct AABB
{
    com::Vec3f min;
    com::Vec3f max;
};

template<typename VERTEX>
AABB CalculateAABB(VERTEX const* const vertices, idx_t const count)
{
    AABB box {};
    for(idx_t i = 0; i < count; ++i)
    {
        auto const& pos = vertices[i].pos;
        if  (box.min.x > pos.x)
             box.min.x = pos.x;
        else box.max.x = pos.x;

        if  (box.min.y > pos.y)
             box.min.y = pos.y;
        else box.max.y = pos.y;

        if  (box.min.z > pos.z)
             box.min.z = pos.z;
        else box.max.z = pos.z;
    }
    return box;
}

//? intersections

struct Ray
{
    com::Vec3f origin;
    com::Vec3f length;
};

inline bool IsPointInsideRect(as_arithmetic auto x, as_arithmetic auto y, const com::Rectf& rect)
{
    return (static_cast<f32>(x) > rect.x && static_cast<f32>(x) < rect.x + rect.width &&
            static_cast<f32>(y) > rect.y && static_cast<f32>(y) < rect.y + rect.height);
}


struct RayAABB_Intersection
{
    f32 fmin = 0;
    f32 fmax = 0;
    explicit operator bool() const
    { 
        dbg::LogInfo(fmin, fmax);
        return fmax > fmin;
    }
};

inline 
RayAABB_Intersection
RayAABB_Intersection(com::Ray const& ray, AABB const& aabb)
{
    auto const length_inv = 1 / ray.length;

    //fractions
    const auto fmin_x = (aabb.min.x - ray.origin.x) / ray.length.x;
    const auto fmax_x = (aabb.max.x - ray.origin.x) / ray.length.x;

    const auto fmin_y = (aabb.min.y - ray.origin.y) / ray.length.y;
    const auto fmax_y = (aabb.max.y - ray.origin.y) / ray.length.y;

    const auto fmin_z = (aabb.min.z - ray.origin.z) / ray.length.z;
    const auto fmax_z = (aabb.max.z - ray.origin.z) / ray.length.z;

    //clamping
    auto fmin_total = fmin_x;
    auto fmax_total = fmax_x;

    fmin_total = Max(fmin_y, fmin_total);
    fmax_total = Min(fmax_y, fmin_total);

    fmin_total = Max(fmin_z, fmin_total);
    fmax_total = Min(fmax_z, fmin_total);

    return { .fmin = fmin_total, .fmax = fmax_total };
}





struct RayTriangle_Intersection
{
     explicit operator bool() const { return false; }
};

inline 
RayTriangle_Intersection
RayTriangle_Intersection(com::Ray const& ray)
{
    return {};
};

}//ns






/*
com::Optional<com::Vec3f> RayPlaneIntersection(
com::Vec3f const (&cornerPoints)[2][2], com::Ray const& ray)
{
    //TODO: ray from mouse point into world
    //TODO: confined to plane space not endless plane
    //TODO: ray against box (height)

    auto const planeVector1 = cornerPoints[0][0] - cornerPoints[0][1];
    auto const planeVector2 = cornerPoints[0][0] - cornerPoints[1][0];
    auto const planeNormal = com::Normalize(com::Cross(planeVector1, planeVector2)) * -1;
    auto const denominator = com::Dot(planeNormal, ray.direction); 
    auto constexpr EPSILON = 1e-6f;
    if (denominator > EPSILON)
    {
        auto const t = com::Dot(cornerPoints[0][0] - ray.origin, planeNormal) / denominator;
        if (t >= 0)
            return { ray.origin + ray.direction * t };
    }
    return {};
}
*/



/*
struct Intersection
{
    com::Vec3f pos;
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
    const com::Vec3f& rayOrigin, 
    const com::Vec3f& rayDir, 
    const com::Vec3f& v0,
    const com::Vec3f& v1,
    const com::Vec3f& v2)
{
    using namespace com;
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
*/