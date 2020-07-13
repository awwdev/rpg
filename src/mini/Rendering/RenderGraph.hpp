//https://github.com/awwdev

#pragma once
#include "mini/Box/Array.hpp"
#include "mini/Rendering/UboData.hpp"

namespace mini::rendering
{
    struct RenderGraph
    {
        box::Array<UniformData_Text, 1000>      ui_ubo;
        box::Array<UniformData_Default, 1000>   default_uboArray;  //1 group -> N inst
        box::Array<UniformGroup, 1000>          default_uboGroups; //1 group == 1 inst type (1 draw call)

        void Clear()
        {
            ui_ubo.Clear();
            default_uboGroups.Clear();
            default_uboArray.Clear();
        }
    };

}//ns