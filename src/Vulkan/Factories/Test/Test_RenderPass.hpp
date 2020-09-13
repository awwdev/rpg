//https://github.com/awwdev

#pragma once
#include "Vulkan/Core.hpp"
#include "Vulkan/Context.hpp"
#include "Vulkan/Objects/RenderPassTest.hpp"

namespace rpg::vk {

inline void Test_CreateRenderPass(RenderPassTest& rp, VkCommandPool cmdPool)
{  
    rp.width  = g_contextPtr->surfaceCapabilities.currentExtent.width;
    rp.height = g_contextPtr->surfaceCapabilities.currentExtent.height;

    const VkAttachmentDescription colorDesc {
        .flags          = 0 ,
        .format         = g_contextPtr->format, 
        .samples        = VK_SAMPLE_COUNT_1_BIT,
        .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
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

    VkSubpassDependency dependencies []
    {
        {
            .srcSubpass         = VK_SUBPASS_EXTERNAL,
            .dstSubpass         = 0,
            .srcStageMask       = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            .dstStageMask       = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask      = VK_ACCESS_SHADER_READ_BIT,
            .dstAccessMask      = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .dependencyFlags    = VK_DEPENDENCY_BY_REGION_BIT,
        },
        {
            .srcSubpass         = 0,
            .dstSubpass         = VK_SUBPASS_EXTERNAL,
            .srcStageMask       = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask       = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            .srcAccessMask      = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .dstAccessMask      = VK_ACCESS_SHADER_READ_BIT,
            .dependencyFlags    = VK_DEPENDENCY_BY_REGION_BIT,
        }
    };

    const VkRenderPassCreateInfo renderPassInfo {
        .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext           = nullptr,
        .flags           = 0,
        .attachmentCount = 1,
        .pAttachments    = &colorDesc,
        .subpassCount    = 1,
        .pSubpasses      = &subpassDesc,
        .dependencyCount = ArrayCount(dependencies),
        .pDependencies   = dependencies
    };

    VkCheck(vkCreateRenderPass(g_contextPtr->device, &renderPassInfo, nullptr, &rp.renderPass));

    //? framebuffer

    rp.renderImage.Create(cmdPool);

    const VkImageView views [] {
        rp.renderImage.view
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
    
    VkCheck(vkCreateFramebuffer(g_contextPtr->device, &framebufferInfo, nullptr, &rp.framebuffer));

}    

}//ns