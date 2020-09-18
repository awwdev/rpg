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

}//ns