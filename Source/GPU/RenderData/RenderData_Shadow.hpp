//https://github.com/awwdev

#pragma once
#include "com/Container/Array.hpp"
#include "com/Matrix.hpp"

namespace rpg::gpu {

struct RenderData_Shadow
{
    static constexpr uint32_t CASCADE_COUNT = 4; // need to change array size in shader too (UBO)

    struct Push_Cascades
    {
        uint32_t cascadeIdx;
    };

    struct UBO_ShadowMap
    {
        com::Mat4f projView       [CASCADE_COUNT];
        com::Mat4f projViewBiased [CASCADE_COUNT];
    };

    //? DATA
    UBO_ShadowMap uboShadowMap;

    void Clear()
    {

    }
};

}//ns