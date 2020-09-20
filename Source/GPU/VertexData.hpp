//https://github.com/awwdev

#pragma once
#include "Common/Structs.hpp"

namespace rpg::gpu {

struct PostVertex
{
    com::Vec2f pos;
    com::Vec2f tex;
    alignas(2) f32 blur;
};

struct GeneralVertex
{
    alignas(16) com::Vec3f pos;
    alignas(16) com::Vec3f nor;
    alignas(16) com::Vec4f col;
    alignas(16) com::Vec2f tex;
};

}//ns