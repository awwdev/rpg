//https://github.com/awwdev

#pragma once
#include "GPU/UboData.hpp"
#include "Common/Structs.hpp"
#include "Resources/MeshTypes.hpp"

namespace rpg::gpu {

constexpr u32 UI_UBO_MAX_COUNT = 2'000; //8*4*2000 < 65.000
constexpr u32 DEFAULT_UBO_MAX_COUNT = 1'000;
constexpr u32 DEFAULT_VERTEX_MAX_COUNT = 5000;
constexpr u32 TERRAIN_VERTEX_MAX_COUNT = 1'000'000;

//keep rendergraph intact until all rendering (since it will be referenced on draw too)
struct RenderData
{
    UniformDataGroups<UI_UniformData, UI_UBO_MAX_COUNT, 1>  gui_ubo; //1 group only
    UniformDataGroupsMesh<Default_UniformData, DEFAULT_UBO_MAX_COUNT> common_ubo;

    static auto constexpr CASCADE_COUNT = 3;

    f32 depthBiasConstantFactor [CASCADE_COUNT] { -2000, -3000, -10000 };
    f32 depthBiasClamp          [CASCADE_COUNT] {    0, 0, 0 };
    f32 depthBiasSlopeFactor    [CASCADE_COUNT] { -0.5, -0.5, -0.5 };

    f32 cascadeZoom     [CASCADE_COUNT] {};
    f32 maxShadowDist   [CASCADE_COUNT] {};

    void Clear()
    {
        gui_ubo.Clear();
        common_ubo.Clear();
    }
};

}//ns