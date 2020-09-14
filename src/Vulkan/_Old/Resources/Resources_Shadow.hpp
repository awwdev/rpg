//https://github.com/awwdev

#pragma once

#include "Vulkan/Meta/Context.hpp"
#include "Resources/CpuResources.hpp"

#include "Vulkan/_Old/Factories/Shadow/Shadow_Shader.hpp"
#include "Vulkan/_Old/Factories/Shadow/Shadow_RenderPass.hpp"

namespace rpg::vuk {

struct Resources_Shadow
{
    RenderPassDepth renderPass;
    Shadow_PushConstants pushConsts;

    void Create(res::HostResources&, VkCommandPool cmdPool)
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