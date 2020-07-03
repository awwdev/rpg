//https://github.com/awwdev
#pragma once

namespace mini
{
    namespace rendergraph 
    {
        struct UniformData_Text
        {
            math::Vec4f offset;
            math::Vec2f size;
            uint32_t    colorIndex;
            uint32_t    textureIndex; 
        };
    }

}//ns