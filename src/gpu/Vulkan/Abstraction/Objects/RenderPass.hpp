//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Abstraction/Meta/Context.hpp"
#include "gpu/Vulkan/Abstraction/Objects/Image.hpp"

namespace rpg::gpu::vuk {

struct RenderPass
{
    VkArray<VkRenderPassBeginInfo, 4> beginInfos;
    VkRenderPass renderPass;
    VkArray<VkFramebuffer, 4> framebuffers;
    VkArray<Image, 4> images;
    uint32_t width, height;

    void Destroy() 
    {
        DestroyItself();
        DestroyOther();
    }
    ~RenderPass() { DestroyItself(); }

private:

    void DestroyOther()
    {
        FOR_VK_ARRAY(images, i)
        images[i].Destroy();
    }

    void DestroyItself()
    {
        vkDestroyRenderPass (g_contextPtr->device, renderPass, nullptr);
        FOR_VK_ARRAY(framebuffers, i)
        vkDestroyFramebuffer(g_contextPtr->device, framebuffers[i], nullptr);
    }

};

}//ns