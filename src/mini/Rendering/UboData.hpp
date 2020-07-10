//https://github.com/awwdev
#pragma once
#include "mini/Math/Matrix.hpp"

namespace mini
{
    namespace rendering 
    {
        struct UniformData_Text
        {
            utils::Rect<float> rect;
            uint32_t colorIndex;
            uint32_t textureIndex; 
            uint32_t padding1;
            uint32_t padding2;
        };

        struct UniformGroup
        {
            u32 begin, count;
        };

        struct UniformData_Default
        {
            math::Mat4f transform;
        };
    }

}//ns