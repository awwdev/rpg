//https://github.com/awwdev

#pragma once

#include "gpu/Vulkan/Resources/Resources_Shaders.hpp"
#include "gpu/Vulkan/Resources/Resources_Pipelines.hpp"

namespace rpg::gpu::vuk {

struct Resources
{
    Resources_Shaders   shaders;
    Resources_Pipelines pipelines;

    void Create()
    {
        shaders.Create();
        pipelines.Create(shaders);
    }
    
    void Destroy()
    {
        pipelines.Destroy();
        shaders.Destroy();
    }
};

}//ns