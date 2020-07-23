//https://github.com/awwdev

#pragma once
#include "mini/Box/Array.hpp"
#include "mini/Rendering/UboData.hpp"
#include "mini/Utils/Structs.hpp"
#include "mini/Resources/Mesh.hpp"
#include "mini/Rendering/Camera.hpp"

namespace mini::rendering
{
    constexpr u32 UI_UBO_MAX_COUNT = 1000;
    constexpr u32 DEFAULT_UBO_MAX_COUNT = 1000;
    constexpr u32 DEFAULT_VERTEX_MAX_COUNT = 1000;
    constexpr u32 TERRAIN_VERTEX_MAX_COUNT = 1000;

    //keep rendergraph intact until all rendering (since it will be referenced on draw too)
    struct RenderGraph
    {
        UniformDataGroups<UI_UniformData, UI_UBO_MAX_COUNT, 1>  ui_ubo; //1 group only
        UniformDataGroups<Default_UniformData, DEFAULT_UBO_MAX_COUNT> default_ubo;

        //Terrain
        //TODO: expandable
        static constexpr auto QUAD_COUNT_Z = 10;
        static constexpr auto QUAD_COUNT_X = 10;
        box::SimpleArray<utils::Common_Vertex, QUAD_COUNT_Z * QUAD_COUNT_X * 6> terrain {};

        Camera camera;

        RenderGraph()
        {
            //terrain = res::CreateMeshGrid<QUAD_COUNT_X, QUAD_COUNT_Z>(100, 100);
        }

        void Clear()
        {
            ui_ubo.Clear();
            default_ubo.Clear();
        }
    };

}//ns