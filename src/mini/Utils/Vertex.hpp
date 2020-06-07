#pragma once

#include "mini/Math/Matrix.hpp"
#include <iostream>

namespace mini
{
    //default vertex struct
    struct Vertex
    {
        math::Vec3f pos;
        math::Vec4f col;
        math::Vec2f tex;
    };

    template<typename T>
    struct Rect
    {
        T x, y, w, h;
    };


    std::ostream& operator<<(std::ostream& os, const Vertex& vert)
    {
        os << "vertex data\n";
        os << "pos " << vert.pos << '\n';
        os << "col " << vert.col << '\n';
        os << "tex " << vert.tex;
        return os;
    }

}//ns