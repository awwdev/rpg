//https://github.com/awwdev
#pragma once

namespace mini
{
    struct UboData_UI
    {
        bool useTexture = false;   
    };

    namespace rendergraph 
    {
        struct UniformData_Text
        {
            math::Vec4f offset;
            uint32_t    colorIndex;
            uint32_t    textureIndex; 
            uint32_t    padding1;
            uint32_t    padding2;
        };
    }


}//ns