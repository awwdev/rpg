//https://github.com/awwdev

#pragma once

#include "gpu/Vulkan/Resources/RenderPasses/RenderPass_Post.hpp"
#include "gpu/Vulkan/Resources/RenderPasses/RenderPass_Main.hpp"
#include "gpu/Vulkan/Resources/RenderPasses/RenderPass_GUI.hpp"
#include "gpu/Vulkan/Resources/RenderPasses/RenderPass_Shadow.hpp"

namespace rpg::gpu::vuk {

struct Resources_RenderPasses
{
    void Create()
    {
        CreateRenderPass_GUI();
        CreateRenderPass_Post();
        CreateRenderPass_Main();
        CreateRenderPass_Shadow();
    }
    
    void Destroy()
    {

    }
};

}//ns