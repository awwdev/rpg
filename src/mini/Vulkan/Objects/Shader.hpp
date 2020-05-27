//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"


namespace mini::vk
{
    struct Base
    {
        VkDevice deviceRef;
        explicit Base(Context& context) : deviceRef { context.device } {;}
    };

    struct Shader : Base
    {
        
        ~Shader()
        {

        }
    };

    //? FACTORIES

    inline Shader CreateShader_Default()
    {

    }

}//ns