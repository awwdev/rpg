//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Objects/Image.hpp"

#include "mini/Box/Array.hpp"

namespace mini::vk
{
    inline VkRenderPassBeginInfo CreateRenderPassBeginInfo(
        VkRenderPass  renderPass,
        VkFramebuffer framebuffer,
        const uint32_t clearValueCount = 0,
        const VkClearValue*  clears = nullptr
    )
    {
        return {
            .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext          = nullptr,
            .renderPass     = renderPass,
            .framebuffer    = framebuffer,
            .renderArea     = {
                .offset     = VkOffset2D {0, 0},
                .extent     = g_contextPtr->surfaceCapabilities.currentExtent
            },
            .clearValueCount= clearValueCount,
            .pClearValues   = clears
        };
    }

    struct RenderPass
    {
        VkRenderPass renderPass;
        box::Array<VkFramebuffer, 4> framebuffers;
        VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT; //!set in factory

        DepthImage depthImage; //optional
        MSAAImage  msaaImage;

        ~RenderPass()
        {
            vkDestroyRenderPass(g_contextPtr->device, renderPass, nullptr);
            FOR_VK_ARRAY(framebuffers, i) 
                vkDestroyFramebuffer(g_contextPtr->device, framebuffers[i], nullptr);
        }
        
    };

    struct RenderPassDepth
    {
        VkRenderPass renderPass;
        VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT; //!set in factory

        VkFramebuffer framebuffer;
        DepthImage    depthImage;

        ~RenderPassDepth()
        {
            vkDestroyRenderPass(g_contextPtr->device, renderPass, nullptr);
            vkDestroyFramebuffer(g_contextPtr->device, framebuffer, nullptr);
        }
    };

}//ns