//https://github.com/awwdev

#pragma once
#include "mini/ECS/ComponentTypes.hpp"
#include "mini/Utils/Matrix.hpp"
#include "mini/Debug/Logger.hpp"

namespace mini::ecs {

struct C_Transform
{
    utils::Mat4f transform;
    utils::Vec3f scale; 

    C_Transform(
        const utils::Mat4f& pTransform = {}, 
        const utils::Vec3f pScale = {})
        : transform { pTransform }
        , scale     { pScale }
    {}

    C_Transform(const ComponentDataStringArray& str)
    {
        FOR_ARRAY(str, i){
            const auto comopnentDataType = GetComponentDataType(str[i]);
        }
        
    }
};

}//NS