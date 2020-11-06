//https://github.com/awwdev

#pragma once

#include "gpu/Vulkan/States/Resources/RenderPasses/RenderPass_Post.hpp"
#include "gpu/Vulkan/States/Resources/RenderPasses/RenderPass_Main.hpp"
#include "gpu/Vulkan/States/Resources/RenderPasses/RenderPass_Shadow.hpp"

namespace rpg::gpu::vuk {

struct Resources_RenderPasses
{
    RenderPass shadowRenderPass;
    RenderPass mainRenderPass;
    RenderPass postRenderPass;

    void Create()
    {
        CreateRenderPass_Shadow(shadowRenderPass);
        CreateRenderPass_Main(mainRenderPass);
        CreateRenderPass_Post(postRenderPass);
    }
    
    void Destroy()
    {
        postRenderPass.Destroy();
        mainRenderPass.Destroy();
        shadowRenderPass.Destroy();
    }
};

}//ns