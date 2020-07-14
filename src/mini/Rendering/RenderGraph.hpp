//https://github.com/awwdev

#pragma once
#include "mini/Box/Array.hpp"
#include "mini/Rendering/UboData.hpp"

namespace mini::rendering
{
    //why not immediately push stuff to vulkan as we go?
    //so we have overhead of copy all render data into a huge render graph object?
    //+ if you would pass it directly to eg Vulkan, there might be sync issues?
    //+ makes is maybe easier for multiple API communication (but could be solve with some dedicated fn)
    //+ collect all stuff on host side, maybe do some processing, and then vulkan comes into play (but what processing would be needed?)
    //+ data is distrbuted over components, if we would immediate store, we had lots of store calls, with rendergraph we can do one store and a big array
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
