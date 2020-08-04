//https://github.com/awwdev

#pragma once
#include "mini/Rendering/UboData.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Resources/Mesh.hpp"

namespace mini::rendering
{
    constexpr u32 UI_UBO_MAX_COUNT = 1000;
    constexpr u32 DEFAULT_UBO_MAX_COUNT = 1000;
    constexpr u32 DEFAULT_VERTEX_MAX_COUNT = 1000;
    constexpr u32 TERRAIN_VERTEX_MAX_COUNT = 1'000'000;

    //keep rendergraph intact until all rendering (since it will be referenced on draw too)
    struct RenderGraph
    {
        UniformDataGroups<UI_UniformData, UI_UBO_MAX_COUNT, 1>  ui_ubo; //1 group only
        UniformDataGroups<Default_UniformData, DEFAULT_UBO_MAX_COUNT> default_ubo;

        bool terrain_wire_mode = true;

        void Clear()
        {
            ui_ubo.Clear();
            default_ubo.Clear();
        }
    };

}//ns