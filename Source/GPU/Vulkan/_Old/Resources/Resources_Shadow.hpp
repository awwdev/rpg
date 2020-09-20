//https://github.com/awwdev

#pragma once

#include "GPU/Vulkan/Meta/Context.hpp"
#include "Resources/CpuResources.hpp"

#include "GPU/Vulkan/_Old/Factories/Shadow/Shadow_Shader.hpp"
#include "GPU/Vulkan/_Old/Factories/Shadow/Shadow_RenderPass.hpp"

namespace rpg::gpu::vuk {

struct Resources_Shadow
{
    RenderPassDepth renderPass;
    Shadow_PushConstants pushConsts;

    void Create(res::CpuResources&, VkCommandPool cmdPool)
    {
        Shadow_CreateRenderPass(renderPass, cmdPool);
    }

    void Recreate(VkCommandPool cmdPool)
    {
        renderPass.Clear();
        Shadow_CreateRenderPass(renderPass, cmdPool);
    }
};

}//NS