//https://github.com/awwdev

#pragma once
#include "Vulkan/Core.hpp"
#include "Vulkan/Context.hpp"
#include "Vulkan/Objects/Image.hpp"
#include "Vulkan/Objects/Image2.hpp"

namespace rpg::vk {

struct RenderPassTest
{
    VkRenderPass renderPass;
    VkSampleCountFlagBits sampleCount;
    VkRenderPassBeginInfo beginInfo;
    uint32_t width, height;

    DepthImage    depthImage;
    Image2        renderImage;
    Image2        msaaImage;

    VkFramebuffer framebuffer;

    ~RenderPassTest()
    {
        Clear();
    }

    void Clear()
    {
        vkDestroyRenderPass (g_contextPtr->device, renderPass, nullptr);
        vkDestroyFramebuffer(g_contextPtr->device, framebuffer, nullptr);
        renderImage.Clear();    
        depthImage.Clear();    
        msaaImage.Clear();
    }

    template<idx_t N>
    VkRenderPassBeginInfo GetBeginInfo(const VkClearValue (&clears) [N])
    {
        return {
            .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext          = nullptr, 
            .renderPass     = renderPass,
            .framebuffer    = framebuffer,
            .renderArea     = {
                .offset     = VkOffset2D {0, 0},
                .extent     = { width, height }
            },
            .clearValueCount= N,
            .pClearValues   = clears
        };
    }
    
};

}//ns