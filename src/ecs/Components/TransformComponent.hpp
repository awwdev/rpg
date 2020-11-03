//https://github.com/awwdev

#pragma once
#include "ecs/EntityID.hpp"
#include "com/Matrix.hpp"
#include "com/box/Array.hpp"

namespace rpg::ecs {

struct TransformComponent
{
    //? children
    static constexpr idx_t CHILDREN_MAX = 10;
    com::Array<ID, CHILDREN_MAX> children;

    //? transform
    com::Vec3f scale        { 1, 1, 1 }; 
    com::Vec3f rotation     { 0, 0, 0 };
    com::Vec3f translation  { 0, 0, 0 };
    com::Mat4f transform    { com::Identity4() }; 
};

}//ns