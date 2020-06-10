#pragma once

#include "mini/Math/Matrix.hpp"

namespace mini
{
    using Color4f = math::Vec4f;

    template<typename T>
    struct Rect
    {
        T x, y, w, h;
    };

    template<typename VAL1, typename VAL2>
    inline auto Max(const VAL1& val1, const VAL2& val2)
    {
        return (val1 > val2) ? val1 : val2;
    }

    template<class POINT, class RECT, 
        typename = IsArithmetic<POINT>, 
        typename = IsArithmetic<RECT>>
    inline bool IsPointInsideRect(const POINT x, const POINT y, const Rect<RECT>& rect)
    {
        return (x > rect.x && x < rect.x + rect.w &&
                y > rect.y && y < rect.y + rect.h);
    }

}//ns