//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"
#include "mini/Vulkan/Objects/RenderPass.hpp"

namespace mini::vk
{
    inline void Shadow_CreateRenderPass(RenderPassDepth& rp, VkCommandPool cmdPool)
    {  
        rp.sampleCount = VK_SAMPLE_COUNT_1_BIT;
        constexpr VkFormat DEPTH_FORMAT = VK_FORMAT_D32_SFLOAT;
        rp.depthImage.Create(
            cmdPool, 
            DEPTH_FORMAT, 
            g_contextPtr->surfaceCapabilities.currentExtent.width,//2048, 
            g_contextPtr->surfaceCapabilities.currentExtent.height,//2048,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            rp.sampleCount);

        const VkAttachmentDescription depthDesc {
            .flags          = 0 ,
            .format         = DEPTH_FORMAT, 
            .samples        = rp.sampleCount,
            .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
        };

        const VkAttachmentReference depthRef {
            .attachment = 0,
            .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        };
        
        const VkSubpassDescription subpassDesc {
            .flags                   = 0,
            .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount    = 0,
            .pInputAttachments       = nullptr,
            .colorAttachmentCount    = 0,
            .pColorAttachments       = nullptr,
            .pResolveAttachments     = nullptr,
            .pDepthStencilAttachment = &depthRef,
            .preserveAttachmentCount = 0,
            .pPreserveAttachments    = nullptr
        };

        const VkAttachmentDescription descs [] {
            depthDesc,
        };

        const VkSubpassDependency dependencies [] =
        {
            {
                .srcSubpass      = VK_SUBPASS_EXTERNAL,
                .dstSubpass      = 0,
                .srcStageMask    = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                .dstStageMask    = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                .srcAccessMask   = VK_ACCESS_SHADER_READ_BIT,
                .dstAccessMask   = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT
            },
            {
                .srcSubpass      = 0,
                .dstSubpass      = VK_SUBPASS_EXTERNAL,
                .srcStageMask    = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                .dstStageMask    = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                .srcAccessMask   = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                .dstAccessMask   = VK_ACCESS_SHADER_READ_BIT,
                .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT
            },
        };

        const VkRenderPassCreateInfo renderPassInfo {
            .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext           = nullptr,
            .flags           = 0,
            .attachmentCount = ARRAY_COUNT(descs),
            .pAttachments    = descs,
            .subpassCount    = 1,
            .pSubpasses      = &subpassDesc,
            .dependencyCount = ARRAY_COUNT(dependencies),
            .pDependencies   = dependencies
        };

        VK_CHECK(vkCreateRenderPass(g_contextPtr->device, &renderPassInfo, nullptr, &rp.renderPass));

        //? framebuffer

        const VkImageView views [] {
            rp.depthImage.view,
        };

        const VkFramebufferCreateInfo framebufferInfo{
            .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext           = nullptr,
            .flags           = 0,
            .renderPass      = rp.renderPass,
            .attachmentCount = ARRAY_COUNT(views),
            .pAttachments    = views,
            .width           = rp.depthImage.width,
            .height          = rp.depthImage.height,
            .layers          = 1
        };
        
        VK_CHECK(vkCreateFramebuffer(g_contextPtr->device, &framebufferInfo, nullptr, &rp.framebuffer));

    }
    

}//ns