//https://github.com/awwdev

#pragma once

#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Objects/RenderPass.hpp"

namespace rpg::gpu::vuk {

inline void UI_CreateRenderPass(RenderPass& rp)
{  
    rp.width  = g_contextPtr->surfaceCapabilities.currentExtent.width;
    rp.height = g_contextPtr->surfaceCapabilities.currentExtent.height;

    const VkAttachmentDescription colorDesc {
        .flags          = 0 ,
        .format         = g_contextPtr->format, 
        .samples        = rp.sampleCount,
        .loadOp         = VK_ATTACHMENT_LOAD_OP_LOAD,//VK_ATTACHMENT_LOAD_OP_LOAD,
        .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout  = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
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

    VkCheck(vkCreateRenderPass(g_contextPtr->device, &renderPassInfo, nullptr, &rp.renderPass));

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
            .attachmentCount = ArrayCount(views),
            .pAttachments    = views,
            .width           = rp.width,
            .height          = rp.height,
            .layers          = 1
        };
        
        VkCheck(vkCreateFramebuffer(g_contextPtr->device, &framebufferInfo, nullptr, &rp.framebuffers[i]));
    }
}    

}//ns