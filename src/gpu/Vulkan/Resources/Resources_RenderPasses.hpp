//https://github.com/awwdev

#pragma once

#include "gpu/Vulkan/Resources/RenderPasses/RenderPass_Post.hpp"
#include "gpu/Vulkan/Resources/RenderPasses/RenderPass_Main.hpp"
#include "gpu/Vulkan/Resources/RenderPasses/RenderPass_GUI.hpp"
#include "gpu/Vulkan/Resources/RenderPasses/RenderPass_Shadow.hpp"

namespace rpg::gpu::vuk {

struct Resources_RenderPasses
{
    RenderPass guiRenderPass;
    RenderPass shadowRenderPass;
    RenderPass postRenderPass;
    RenderPass mainRenderPass;

    void Create()
    {
        CreateRenderPass_GUI(guiRenderPass);
        CreateRenderPass_Post(postRenderPass);
        CreateRenderPass_Main(mainRenderPass);
        CreateRenderPass_Shadow(shadowRenderPass);
    }
    
    void Destroy()
    {
        guiRenderPass.Destroy();
        shadowRenderPass.Destroy();
        postRenderPass.Destroy();
        mainRenderPass.Destroy();
    }
};

}//ns