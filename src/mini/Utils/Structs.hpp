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

    template<typename T>
    struct Rect
    {
        T x, y, w, h;
    };

}//ns