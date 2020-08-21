//https://github.com/awwdev

#pragma once
#include "mini/ECS/Components.hpp"
#include "mini/Utils/Matrix.hpp"
#include "mini/Debug/Logger.hpp"

namespace mini::ecs {

struct C_Transform
{
    utils::Mat4f transform;

    C_Transform() = default;
    C_Transform(const utils::Mat4f& pTransform)
        : transform { pTransform }
    {}

    C_Transform(const ComponentDataStrings& str)
    {

    }
};

}//NS