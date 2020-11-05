//https://github.com/awwdev

#pragma once

#include "gpu/Vulkan/Resources/Resources_Shaders.hpp"
#include "gpu/Vulkan/Resources/Resources_Pipelines.hpp"
#include "gpu/Vulkan/Resources/Resources_RenderPasses.hpp"

namespace rpg::gpu::vuk {

struct Resources
{
    Resources_Shaders      shaders;
    Resources_RenderPasses renderPasses;
    Resources_Pipelines    pipelines;

    void Create()
    {
        shaders.Create();
        renderPasses.Create();
        pipelines.Create(shaders);
    }
    
    void Destroy()
    {
        pipelines.Destroy();
        renderPasses.Destroy();
        shaders.Destroy();
    }
};

}//ns