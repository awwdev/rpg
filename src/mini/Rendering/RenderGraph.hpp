//https://github.com/awwdev

#pragma once
#include "mini/Box/Array.hpp"
#include "mini/Rendering/UboData.hpp"

namespace mini::rendering
{
    constexpr u32 UI_UBO_MAX_COUNT = 1000;
    constexpr u32 DEFAULT_UBO_MAX_COUNT = 1000;
    constexpr u32 DEFAULT_VERTEX_MAX_COUNT = 1000;

    //keep rendergraph intact until all rendering (since it will be referenced on draw too)
    struct RenderGraph
    {
        //array data and groups (views) to manage drawing
        UniformDataGroups<UniformData_UI, UI_UBO_MAX_COUNT, 1>  ui_ubo;
        UniformDataGroups<UniformData_Default, DEFAULT_UBO_MAX_COUNT> default_ubo;

        void Clear()
        {
            ui_ubo.Clear();
            default_ubo.Clear();
        }
    };

}//ns