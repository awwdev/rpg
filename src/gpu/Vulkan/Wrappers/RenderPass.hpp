//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Context.hpp"
#include "gpu/Vulkan/Wrappers/Image.hpp"
#include "com/box/SimpleArray.hpp"

namespace rpg::gpu::vuk {

struct RenderPass
{
    VkRenderPass renderPass;
    
    com::SimpleArrayVk<VkRenderPassBeginInfo, 4> beginInfos;
    com::SimpleArrayVk<VkFramebuffer, 4> framebuffers;
    com::SimpleArrayVk<VkClearValue, 4> clears;
    com::SimpleArrayVk<Image, 4> images;
    com::SimpleArrayVk<VkImageView, 4> views; //useful for img array

    VkFormat colorFormat, depthFormat;
    uint32_t width, height;
    VkSampleCountFlagBits msaaSampleCount = VK_SAMPLE_COUNT_1_BIT;

    void Destroy() 
    {
        DestroyOwnResources();
        DestroyOtherResources();
    }
    ~RenderPass() { DestroyOwnResources(); }

private:

    void DestroyOtherResources()
    {
        FOR_SIMPLE_ARRAY(images, i)
            images[i].Destroy();
        images.ResetCount();
    }

    void DestroyOwnResources()
    {
        vkDestroyRenderPass (g_contextPtr->device, renderPass, nullptr);
        FOR_SIMPLE_ARRAY(framebuffers, i)
            vkDestroyFramebuffer(g_contextPtr->device, framebuffers[i], nullptr);
        framebuffers.ResetCount();
        FOR_SIMPLE_ARRAY(views, i)
            vkDestroyImageView(g_contextPtr->device, views[i], nullptr);
        views.ResetCount();
        FOR_SIMPLE_ARRAY(beginInfos, i)
            beginInfos[i] = {};
        beginInfos.ResetCount();

    }

};

}//ns