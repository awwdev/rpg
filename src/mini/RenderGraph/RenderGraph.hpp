//https://github.com/awwdev

#pragma once
#include "mini/Box/Array.hpp"
#include "mini/RenderGraph/UboData.hpp"

namespace mini::rendergraph
{
    struct RenderGraph
    {
        box::Array<UniformData_Text, 1000> ubo_ui;

        box::Array<UniformGroup, 1000>          default_uboGroups;
        box::Array<UniformData_Default, 1000>   default_uboArray;

        void Clear()
        {
            ubo_ui.Clear();
            //probably clear when needed at the dedicated place, so it is more transparent
            //also we may not want to clear all, but only specific stuff to replace
        }
    };

}//ns