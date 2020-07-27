//https://github.com/awwdev

#pragma once
#include "mini/Utils/Structs.hpp"
#include "mini/Utils/Algorithms.hpp"

namespace mini::rendering
{
    struct Sun
    {
        math::Vec3f pos { -4, -1, -4 };
        float t = 0;

        void Update(const double dt)
        {
            using namespace math;
            t += dt * 0.5f;
            pos[X] = std::sinf(t);
            pos[Z] = std::cosf(t);
        }

        math::Mat4f GetView() const
        {
            return math::LookAt(pos, {0, 0, 0});
        }
    };

}//ns