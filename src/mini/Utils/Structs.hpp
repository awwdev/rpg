#pragma once

#include "mini/Math/Matrix.hpp"
#include <iostream>

namespace mini::utils
{
    //default vertex struct
    struct Vertex
    {
        math::Vec3f pos;
        math::Vec4f col;
        math::Vec2f tex;
    };

    std::ostream& operator<<(std::ostream& os, const utils::Vertex& vert)
    {
        os << "vertex data\n";
        os << "pos " << vert.pos << '\n';
        os << "col " << vert.col << '\n';
        os << "tex " << vert.tex;
        return os;
    }

    using Color4f = math::Vec4f;
    using Color4u = math::Vec<u8, 4>;

    
    inline Color4f NormaliseColor(const Color4u& col) { 
        return { 
            col[Vx] / 255.f, 
            col[Vy] / 255.f, 
            col[Vz] / 255.f, 
            col[Vw] / 255.f 
        };
    }

    inline Color4u HighlightColor(const Color4u& col, const u8 amount)
    {
        Color4u out { col };
        out[Vx] += amount;
        out[Vy] += amount;
        out[Vz] += amount;
        return out;
    }

    template<typename T>
    struct Rect
    {
        T x, y, w, h;
    };



}//ns