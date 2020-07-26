//https://github.com/awwdev

#pragma once
#include "mini/Utils/Structs.hpp"
#include "mini/Utils/Algorithms.hpp"

namespace mini::rendering
{
    struct Sun
    {
        math::Vec3f pos { -4, -4, -4 };

        math::Mat4f GetView() const
        {
            return math::LookAt(pos, {0, 0, 0});
        }
    };

}//ns