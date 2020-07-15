//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Objects/RenderPass.hpp"

namespace mini::vk
{
    inline void CreateRenderPass_UI(RenderPass& rp)
    {  
        const VkAttachmentDescription colorDesc {
            .flags          = 0 ,
            .format         = g_contextPtr->format, 
            .samples        = rp.SAMPLE_COUNT,
            .loadOp         = VK_ATTACHMENT_LOAD_OP_LOAD,
            .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout  = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            .finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        };

        const VkAttachmentReference colorRef {
            .attachment = 0,
            .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };
        
        const VkSubpassDescription subpassDesc {
            .flags                   = 0,
            .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount    = 0,
            .pInputAttachments       = nullptr,
            .colorAttachmentCount    = 1,
            .pColorAttachments       = &colorRef,
            .pResolveAttachments     = nullptr,
            .pDepthStencilAttachment = nullptr,
            .preserveAttachmentCount = 0,
            .pPreserveAttachments    = nullptr
        };

        const VkRenderPassCreateInfo renderPassInfo {
            .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext           = nullptr,
            .flags           = 0,
            .attachmentCount = 1,
            .pAttachments    = &colorDesc,
            .subpassCount    = 1,
            .pSubpasses      = &subpassDesc,
            .dependencyCount = 0,
            .pDependencies   = nullptr
        };

        VK_CHECK(vkCreateRenderPass(g_contextPtr->device, &renderPassInfo, nullptr, &rp.renderPass));

        //? framebuffers
        for (u32 i = 0; i < g_contextPtr->swapImages.count; ++i)
        {
            rp.framebuffers.Append();

            const VkImageView views [] {
                g_contextPtr->swapImageViews[i],
            };

            const VkFramebufferCreateInfo framebufferInfo{
                .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext           = nullptr,
                .flags           = 0,
                .renderPass      = rp.renderPass,
                .attachmentCount = ARRAY_COUNT(views),
                .pAttachments    = views,
                .width           = g_contextPtr->surfaceCapabilities.currentExtent.width,
                .height          = g_contextPtr->surfaceCapabilities.currentExtent.height,
                .layers          = 1
            };
            
            VK_CHECK(vkCreateFramebuffer(g_contextPtr->device, &framebufferInfo, nullptr, &rp.framebuffers[i]));
        }
    }
    

}//ns