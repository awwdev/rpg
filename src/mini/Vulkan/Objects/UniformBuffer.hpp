//https://github.com/awwdev

#pragma once

#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Objects/Buffer.hpp"

#include "mini/Box/Array.hpp"


namespace mini::vk
{
    //! image info vs buffer info

    struct UniformBuffer
    {
        Buffer buffer;
        
        inline void Create(Context& context)
        {

        }

        void Store()
        {
            //buffer.Store(vertices, sizeof(vertices));
        }

    };


}//ns