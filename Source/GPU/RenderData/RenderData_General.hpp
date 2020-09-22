//https://github.com/awwdev

#pragma once
#include "Common/Container/Array.hpp"
#include "Common/Matrix.hpp"

namespace rpg::gpu {

struct RenderData_General
{
    static constexpr uint32_t VBO_TERRAIN_MAX = 10'000;
    static constexpr uint32_t VBO_OBJECTS_MAX = 10'000;

    struct Vertex
    {
        alignas(16) com::Vec3f pos;
        alignas(16) com::Vec3f nor;
        alignas(16) com::Vec4f col;
        alignas(16) com::Vec2f tex;
    };

    struct UBO_Meta
    {
        alignas(64) com::Mat4f view;
        alignas(64) com::Mat4f proj;
    };

    //? DATA
    UBO_Meta uboData_general_meta; //camera data

    void Clear()
    {
        
    }
};

}//ns