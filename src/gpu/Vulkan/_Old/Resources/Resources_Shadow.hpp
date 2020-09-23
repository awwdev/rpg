//https://github.com/awwdev

#pragma once

#include "gpu/Vulkan/Meta/Context.hpp"
#include "res/_Old/CpuResources.hpp"

#include "gpu/Vulkan/_Old/Factories/Shadow/Shadow_Shader.hpp"
#include "gpu/Vulkan/_Old/Factories/Shadow/Shadow_RenderPass.hpp"

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