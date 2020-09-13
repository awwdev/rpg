#pragma once

#include "Common/Matrix.hpp"
#include <iostream>

namespace rpg::com {
    
using NormColor4f = com::Vec4f;
using RGBAColor4u = com::Vec<u8, 4>;

struct Common_Vertex 
{
    alignas(16) com::Vec3f pos;
    alignas(16) com::Vec3f nor;
    alignas(16) com::Vec4f col;
    alignas(16) com::Vec2f tex; //TODO: rename to uv?

    constexpr Common_Vertex(
        const com::Vec3f pPos = {},
        const com::Vec3f pNor = {},
        const com::Vec4f pCol = {},
        const com::Vec2f pTex = {})
        : pos { pPos }
        , nor { pNor }
        , col { pCol }
        , tex { pTex }
    {}
};

struct Post_Vertex
{
    alignas(16) com::Vec2f pos;
    alignas(16) com::Vec2f tex;
    alignas(16) int32_t blur;
};

std::ostream& operator<<(std::ostream& os, const Common_Vertex& vert)
{
    PrintMatrix(vert.pos);
    PrintMatrix(vert.col);
    PrintMatrix(vert.nor);
    PrintMatrix(vert.tex);
    return os;
}

inline NormColor4f NormaliseColor(const RGBAColor4u& col) { 
    using namespace com;
    return { //normalise on gpu always instead??
        col[X] / 255.f, 
        col[Y] / 255.f, 
        col[Z] / 255.f, 
        col[W] / 255.f 
    };
}

inline RGBAColor4u HighlightColor(const RGBAColor4u& col, const u8 amount)
{
    using namespace com;
    RGBAColor4u out { col };
    out[X] += amount;
    out[Y] += amount;
    out[Z] += amount;
    return out;
}

template<typename T>
struct Rect
{
    T x, y, w, h;

    Rect() = default;
    Rect(const T pX, const T pY, const T pW, const T pH)
        : x { pX }
        , y { pY }
        , w { pW }
        , h { pH }
    {;}

    template<class OTHER_T>
    Rect(const Rect<OTHER_T>& other)
    {
        x = (T)other.x;
        y = (T)other.y;
        w = (T)other.w;
        h = (T)other.h;
    }
};

}//ns