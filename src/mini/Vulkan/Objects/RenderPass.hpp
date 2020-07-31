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
        const VkExtent2D& extent,
        const uint32_t clearValueCount = 0,
        const VkClearValue* clears = nullptr)
    {
        return {
            .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext          = nullptr,
            .renderPass     = renderPass,
            .framebuffer    = framebuffer,
            .renderArea     = {
                .offset     = VkOffset2D {0, 0},
                .extent     = extent
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
        VkRenderPassBeginInfo beginInfo; //!set in factory
        uint32_t width, height;

        DepthImage depthImage; //optional
        MSAAImage  msaaImage;

        ~RenderPass()
        {
            vkDestroyRenderPass(g_contextPtr->device, renderPass, nullptr);
            FOR_ARRAY(framebuffers, i) 
                vkDestroyFramebuffer(g_contextPtr->device, framebuffers[i], nullptr);
            framebuffers.Clear(); //clear because dtor is called on recreation
        }

        VkRenderPassBeginInfo GetBeginInfo(
            const u32 framBufferIdx, 
            const u32 clearCount = 0, 
            const VkClearValue* clears = nullptr) 
        {
            return {
                .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .pNext          = nullptr,
                .renderPass     = renderPass,
                .framebuffer    = framebuffers[framBufferIdx],
                .renderArea     = {
                    .offset     = VkOffset2D {0, 0},
                    .extent     = { width, height }
                },
                .clearValueCount= clearCount,
                .pClearValues   = clears
            };
        }
        
    };

    struct RenderPassDepth
    {
        VkRenderPass renderPass;
        VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT; //!set in factory
        VkRenderPassBeginInfo beginInfo; //!set in factory
        uint32_t width, height;

        VkFramebuffer framebuffer;
        DepthImage    depthImage;

         VkRenderPassBeginInfo GetBeginInfo(
            const u32 clearCount = 0, 
            const VkClearValue* clears = nullptr) 
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
                .clearValueCount= clearCount,
                .pClearValues   = clears
            };
        }

        ~RenderPassDepth()
        {
            vkDestroyRenderPass(g_contextPtr->device, renderPass, nullptr);
            vkDestroyFramebuffer(g_contextPtr->device, framebuffer, nullptr);
        }
    };

}//ns