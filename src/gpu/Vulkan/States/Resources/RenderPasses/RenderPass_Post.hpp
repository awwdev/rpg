//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Wrappers/RenderPass.hpp"
#include "com/Types.hpp"

namespace rpg::gpu::vuk {

inline void CreateRenderPass_Post(RenderPass& rp)
{
    rp.width  = g_contextPtr->surfaceCapabilities.currentExtent.width;
    rp.height = g_contextPtr->surfaceCapabilities.currentExtent.height;
    rp.colorFormat = g_contextPtr->format;

    //? attachement
    VkAttachmentDescription const colorDesc {
        .flags          = 0,
        .format         = rp.colorFormat, 
        .samples        = VK_SAMPLE_COUNT_1_BIT,
        .loadOp         = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    };

    VkAttachmentDescription const descs [] {
        colorDesc,
    };

    VkAttachmentReference const colorRef {
        .attachment = 0,
        .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    //? subpass
    VkSubpassDescription const subpassDesc {
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

    VkSubpassDependency const dependencies []
    {
        {
            .srcSubpass         = VK_SUBPASS_EXTERNAL,
            .dstSubpass         = 0,
            .srcStageMask       = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask       = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask      = 0,
            .dstAccessMask      = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .dependencyFlags    = VK_DEPENDENCY_BY_REGION_BIT,
        },
    };

    //? renderpass
    VkRenderPassCreateInfo const renderPassInfo {
        .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext           = nullptr,
        .flags           = 0,
        .attachmentCount = array_extent(descs),
        .pAttachments    = descs,
        .subpassCount    = 1,
        .pSubpasses      = &subpassDesc,
        .dependencyCount = array_extent(dependencies),
        .pDependencies   = dependencies
    };
    VkCheck(vkCreateRenderPass(g_contextPtr->device, &renderPassInfo, nullptr, &rp.renderPass));

    //? framebuffers
    FOR_SIMPLE_ARRAY(g_contextPtr->swapImages, i)
    {
        const VkImageView views [] {
            g_contextPtr->swapImageViews[i]
        };

        const VkFramebufferCreateInfo framebufferInfo {
            .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext           = nullptr,
            .flags           = 0,
            .renderPass      = rp.renderPass,
            .attachmentCount = array_extent(views),
            .pAttachments    = views,
            .width           = rp.width,
            .height          = rp.height,
            .layers          = 1
        };
        
        rp.framebuffers.Append();
        VkCheck(vkCreateFramebuffer(g_contextPtr->device, &framebufferInfo, nullptr, &rp.framebuffers[i]));
    }

    //? begin info
    FOR_SIMPLE_ARRAY(g_contextPtr->swapImages, i) {
        rp.beginInfos.Append(VkRenderPassBeginInfo {
            .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext          = nullptr, 
            .renderPass     = rp.renderPass,
            .framebuffer    = rp.framebuffers[i],
            .renderArea     = {
                .offset     = VkOffset2D {0, 0},
                .extent     = { rp.width, rp.height }
            },
            .clearValueCount= 0,
            .pClearValues   = nullptr
        });
    }

}

}//ns