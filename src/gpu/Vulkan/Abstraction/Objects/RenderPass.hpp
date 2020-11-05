//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Abstraction/Meta/Context.hpp"

namespace rpg::gpu::vuk {

struct RenderPass
{
    VkArray<VkRenderPassBeginInfo, 4> beginInfos;
    VkRenderPass renderPass;
    uint32_t width, height;

    void Destroy() 
    {
        vkDestroyRenderPass (g_contextPtr->device, renderPass, nullptr);
        //vkDestroyFramebuffer(g_contextPtr->device, framebuffer, nullptr);
    }
    ~RenderPass() { Destroy(); }
};

}//ns