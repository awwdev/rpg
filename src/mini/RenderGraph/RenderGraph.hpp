//https://github.com/awwdev

#pragma once

#include "mini/Box/Array.hpp"
#include "mini/RenderGraph/UboData.hpp"

namespace mini::rendergraph
{
    struct RenderGraph
    {
        //TODO: way to sync count from host to gpu resource (size)
        box::Array<UniformData_Text, 1000> uboText;

        void Clear()
        {
            uboText.Clear();
        }
    };

}//ns


            
