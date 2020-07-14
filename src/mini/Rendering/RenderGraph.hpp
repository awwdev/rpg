//https://github.com/awwdev

#pragma once
#include "mini/Box/Array.hpp"
#include "mini/Rendering/UboData.hpp"

namespace mini::rendering
{
    struct RenderGraph
    {
        //array data and groups (views) to manage drawing
        box::Array<UniformData_Text, 1000>  ui_ubo; //no groups needed
        UniformDataGroups<UniformData_Default, 1000> default_ubo;

        void Clear()
        {
            ui_ubo.Clear();
            default_ubo.Clear();
        }
    };

}//ns