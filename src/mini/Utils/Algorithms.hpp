#pragma once

#include "mini/Utils/Structs.hpp"

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

}//ns