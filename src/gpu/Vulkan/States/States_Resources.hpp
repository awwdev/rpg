//https://github.com/awwdev

#pragma once

#include "gpu/Vulkan/States/Resources/Resources_Shaders.hpp"
#include "gpu/Vulkan/States/Resources/Resources_Pipelines.hpp"
#include "gpu/Vulkan/States/Resources/Resources_RenderPasses.hpp"
#include "gpu/Vulkan/States/Resources/Resources_Uniforms.hpp"
#include "gpu/Vulkan/States/Resources/Resources_Vertices.hpp"

namespace rpg::gpu::vuk {

struct Resources
{
    Resources_Shaders      shaders;
    Resources_RenderPasses renderPasses;
    Resources_Vertices     vertices;
    Resources_Uniforms     uniforms;
    Resources_Pipelines    pipelines;

    void Create()
    {
        shaders.Create();
        renderPasses.Create();
        //vertices.Create();
        //uniforms.Create();
        //pipelines.Create(shaders, renderPasses, uniforms, vertices);
    }
    
    void Destroy()
    {
        //pipelines.Destroy();
        //uniforms.Destroy();
        //vertices.Destroy();
        renderPasses.Destroy();
        shaders.Destroy();
    }
};

}//ns