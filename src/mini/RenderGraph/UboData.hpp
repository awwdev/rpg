//https://github.com/awwdev
#pragma once

namespace mini
{
    namespace rendergraph 
    {
        struct UniformData_Text
        {
            utils::Rect<float> rect;
            uint32_t colorIndex;
            uint32_t textureIndex; 
            uint32_t padding1;
            uint32_t padding2;
        };
    }

}//ns