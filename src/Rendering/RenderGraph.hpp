//https://github.com/awwdev

#pragma once
#include "Rendering/UboData.hpp"
#include "Utils/Structs.hpp"
#include "Resources/MeshTypes.hpp"

namespace rpg::rendering
{
    constexpr u32 UI_UBO_MAX_COUNT = 1000;
    constexpr u32 DEFAULT_UBO_MAX_COUNT = 1000;
    constexpr u32 DEFAULT_VERTEX_MAX_COUNT = 5000;
    constexpr u32 TERRAIN_VERTEX_MAX_COUNT = 1'000'000;

    //keep rendergraph intact until all rendering (since it will be referenced on draw too)
    struct RenderGraph
    {
        UniformDataGroups<UI_UniformData, UI_UBO_MAX_COUNT, 1>  ui_ubo; //1 group only
        UniformDataGroupsMesh<Default_UniformData, DEFAULT_UBO_MAX_COUNT> common_ubo;

        f32 depthBiasConstantFactor = 0;
        f32 depthBiasClamp = 0;
        f32 depthBiasSlopeFactor = 0;

        f32 cascadeZoom0 = 0;
        f32 cascadeZoom1 = 0;
        f32 cascadeZoom2 = 0;
        f32 maxShadowDist0 = 0;
        f32 maxShadowDist1 = 0;
        f32 maxShadowDist2 = 0;

        void Clear()
        {
            ui_ubo.Clear();
            common_ubo.Clear();
        }
    };

}//ns