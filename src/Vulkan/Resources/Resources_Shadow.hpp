//https://github.com/awwdev

#pragma once
#include "Vulkan/Core.hpp"
#include "Vulkan/Context.hpp"
#include "Resources/HostResources.hpp"

#include "Vulkan/Factories/Shadow/Shadow_Shader.hpp"
#include "Vulkan/Factories/Shadow/Shadow_RenderPass.hpp"

namespace rpg::vk {

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