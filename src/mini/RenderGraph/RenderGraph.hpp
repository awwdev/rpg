//https://github.com/awwdev

#pragma once

#include "mini/Box/Array.hpp"
#include "mini/RenderGraph/UboData.hpp"

namespace mini::rendergraph
{
    struct RenderGraph
    {
        box::Array<UniformData_Text, 100> uboText;

        void Clear()
        {
            uboText.Clear();
        }
    };

}//ns