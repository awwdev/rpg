//https://github.com/awwdev

#pragma once
#include "com/box/Array.hpp"
#include "com/Matrix.hpp"

namespace rpg::gpu {

struct RenderData_Shadow
{
    static constexpr uint32_t CASCADE_COUNT = 4; // need to change array size in shader too (UBO)
    static constexpr uint32_t SHADOW_MAP_SIZE = 4 * 1024;

    struct Push_Cascades
    {
        uint32_t cascadeIdx;
    };

    struct UBO_ShadowMap
    {
        com::Mat4f projView       [CASCADE_COUNT];
        com::Mat4f projViewBiased [CASCADE_COUNT];
        alignas(16) com::Vec3f sunDir;
    };

    //? DATA
    UBO_ShadowMap uboShadowMap;

    void Clear()
    {

    }
};

}//ns