#pragma once

#include "mini/Math/Matrix.hpp"

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

}//ns