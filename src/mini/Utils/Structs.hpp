#pragma once

#include "mini/Math/Matrix.hpp"
#include <iostream>

namespace mini::utils
{
    using NormColor4f = math::Vec4f;
    using RGBAColor4u = math::Vec<u8, 4>;

    struct Common_Vertex //could be used by physics too, so not rendering only?
    {
        math::Vec4f pos;
        math::Vec4f nor;
        math::Vec4f col;
        math::Vec4f tex;
    };

    std::ostream& operator<<(std::ostream& os, const Common_Vertex& vert)
    {
        os << "vertex data\n" <<
        "pos " << vert.pos << '\n' <<
        "nor " << vert.nor << '\n' <<
        "col " << vert.col << '\n' <<
        "tex " << vert.tex;
        return os;
    }

    inline NormColor4f NormaliseColor(const RGBAColor4u& col) { 
        return { //normalise on gpu always instead??
            col[Vx] / 255.f, 
            col[Vy] / 255.f, 
            col[Vz] / 255.f, 
            col[Vw] / 255.f 
        };
    }

    inline RGBAColor4u HighlightColor(const RGBAColor4u& col, const u8 amount)
    {
        RGBAColor4u out { col };
        out[Vx] += amount;
        out[Vy] += amount;
        out[Vz] += amount;
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