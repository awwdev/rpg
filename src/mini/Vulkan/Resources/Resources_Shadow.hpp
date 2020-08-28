//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Resources/HostResources.hpp"

#include "mini/Vulkan/Factories/Shadow/Shadow_Shader.hpp"
#include "mini/Vulkan/Factories/Shadow/Shadow_RenderPass.hpp"

namespace mini::vk {

struct Resources_Shadow
{
    RenderPassDepth renderPass;

    void Create(res::HostResources& hostRes, VkCommandPool cmdPool)
    {
        Shadow_CreateRenderPass(renderPass, cmdPool);
    }

    void Recreate(VkCommandPool cmdPool)
    {
        renderPass.~RenderPassDepth();
        Shadow_CreateRenderPass(renderPass, cmdPool);
    }
};

}//NS