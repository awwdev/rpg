//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Abstraction/Meta/Context.hpp"
#include "gpu/Vulkan/Abstraction/Objects/Image.hpp"
#include "com/box/SimpleArray.hpp"

namespace rpg::gpu::vuk {

struct RenderPass
{
    com::SimpleArrayVk<VkRenderPassBeginInfo, 4> beginInfos;
    VkRenderPass renderPass;
    com::SimpleArrayVk<VkFramebuffer, 4> framebuffers;
    com::SimpleArrayVk<Image, 4> images;
    uint32_t width, height;
    VkClearValue clears [1] { { .depthStencil { 0, 0 } } };

    void Destroy() 
    {
        DestroyItself();
        DestroyOther();
    }
    ~RenderPass() { DestroyItself(); }

private:

    void DestroyOther()
    {
        FOR_SIMPLE_ARRAY(images, i)
            images[i].Destroy();
        images.ResetCount();
    }

    void DestroyItself()
    {
        vkDestroyRenderPass (g_contextPtr->device, renderPass, nullptr);
        FOR_SIMPLE_ARRAY(framebuffers, i)
            vkDestroyFramebuffer(g_contextPtr->device, framebuffers[i], nullptr);
        framebuffers.ResetCount();
    }

};

}//ns