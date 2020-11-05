//https://github.com/awwdev

#pragma once

#include "gpu/Vulkan/Resources/Resources_Shaders.hpp"

namespace rpg::gpu::vuk {

struct Resources
{
    Resources_Shaders shaders;

    void Create()
    {
        shaders.Create();
    }
    void Destroy()
    {
        shaders.Destroy();
    }
};

}//ns