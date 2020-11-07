//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Context.hpp"
#include "com/box/SimpleArray.hpp"

namespace rpg::gpu::vuk {

struct GUI_RenderPass
{
    com::SimpleArrayVk<VkRenderPassBeginInfo, 4> beginInfos;
    com::SimpleArrayVk<VkFramebuffer, 4> framebuffers;
    VkRenderPass renderPass;
    uint32_t width, height;

    void Create()
    {
        const VkFormat COLOR_FORMAT = g_contextPtr->format;
        width  = g_contextPtr->surfaceCapabilities.currentExtent.width;
        height = g_contextPtr->surfaceCapabilities.currentExtent.height;

        //? ATTACHMENTS
        const VkAttachmentDescription colorDesc {
            .flags          = 0 ,
            .format         = COLOR_FORMAT, 
            .samples        = VK_SAMPLE_COUNT_1_BIT,
            .loadOp         = VK_ATTACHMENT_LOAD_OP_LOAD,       //load post fx
            .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout  = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        };

        const VkAttachmentDescription descs [] {
            colorDesc,
        };

        const VkAttachmentReference colorRef {
            .attachment = 0,
            .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };
        
        //? SUBPASS
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
                .srcStageMask       = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask       = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .srcAccessMask      = 0,
                .dstAccessMask      = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dependencyFlags    = VK_DEPENDENCY_BY_REGION_BIT,
            },
        };

        //? RENDERPASS
        const VkRenderPassCreateInfo renderPassInfo {
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
        VkCheck(vkCreateRenderPass(g_contextPtr->device, &renderPassInfo, nullptr, &renderPass));

        //? FRAMEBUFFER
        FOR_SIMPLE_ARRAY(g_contextPtr->swapImages, i)
        {
            const VkImageView views [] {
                g_contextPtr->swapImageViews[i]
            };

            const VkFramebufferCreateInfo framebufferInfo {
                .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext           = nullptr,
                .flags           = 0,
                .renderPass      = renderPass,
                .attachmentCount = array_extent(views),
                .pAttachments    = views,
                .width           = width,
                .height          = height,
                .layers          = 1
            };
            
            framebuffers.Append();
            VkCheck(vkCreateFramebuffer(g_contextPtr->device, &framebufferInfo, nullptr, &framebuffers[i]));
        }

        //? BEGIN INFO
        FOR_SIMPLE_ARRAY(g_contextPtr->swapImages, i) {
            beginInfos.Append(VkRenderPassBeginInfo{
                .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .pNext          = nullptr, 
                .renderPass     = renderPass,
                .framebuffer    = framebuffers[i],
                .renderArea     = {
                    .offset     = VkOffset2D {0, 0},
                    .extent     = { width, height }
                },
                .clearValueCount= 0,
                .pClearValues   = nullptr
            });
        }
    }


    void Destroy()
    {
        vkDestroyRenderPass (g_contextPtr->device, renderPass, nullptr);
        FOR_SIMPLE_ARRAY(framebuffers, i)
            vkDestroyFramebuffer(g_contextPtr->device, framebuffers[i], nullptr);
        framebuffers.ResetCount();
        beginInfos.ResetCount();
    }

    ~GUI_RenderPass()
    {
        Destroy();
    }

};

}//ns