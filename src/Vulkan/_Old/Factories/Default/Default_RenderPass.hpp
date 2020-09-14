//https://github.com/awwdev

#pragma once

#include "Vulkan/Meta/Context.hpp"
#include "Vulkan/Objects/RenderPassTest.hpp"

namespace rpg::vuk {

inline void Common_CreateRenderPass(RenderPassTest& rp, VkCommandPool cmdPool)
{  
    rp.sampleCount = VK_SAMPLE_COUNT_4_BIT;
    rp.width  = g_contextPtr->surfaceCapabilities.currentExtent.width;
    rp.height = g_contextPtr->surfaceCapabilities.currentExtent.height;
    constexpr VkFormat DEPTH_FORMAT = VK_FORMAT_D32_SFLOAT;
    constexpr VkFormat COLOR_FORMAT = VK_FORMAT_R8G8B8A8_UNORM;

    //? ATTACHMENTS
    const VkAttachmentDescription colorDesc {
        .flags          = 0 ,
        .format         = COLOR_FORMAT, 
        .samples        = rp.sampleCount,
        .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    };

    const VkAttachmentDescription depthDesc {
        .flags          = 0 ,
        .format         = DEPTH_FORMAT, 
        .samples        = rp.sampleCount,
        .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,        
    };

    const VkAttachmentDescription resolveDesc {
        .flags          = 0 ,
        .format         = COLOR_FORMAT, 
        .samples        = VK_SAMPLE_COUNT_1_BIT,
        .loadOp         = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,        
    };

    const VkAttachmentDescription descs [] {
        colorDesc,
        depthDesc,
        resolveDesc,
    };

    const VkAttachmentReference colorRef {
        .attachment = 0,
        .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };
    const VkAttachmentReference depthRef {
        .attachment = 1,
        .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };
    const VkAttachmentReference resolveRef {
        .attachment = 2,
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
        .pResolveAttachments     = &resolveRef,
        .pDepthStencilAttachment = &depthRef,
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

    //? RENDERPASS
    const VkRenderPassCreateInfo renderPassInfo {
        .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext           = nullptr,
        .flags           = 0,
        .attachmentCount = ArrayCount(descs),
        .pAttachments    = descs,
        .subpassCount    = 1,
        .pSubpasses      = &subpassDesc,
        .dependencyCount = ArrayCount(dependencies),
        .pDependencies   = dependencies
    };
    VkCheck(vkCreateRenderPass(g_contextPtr->device, &renderPassInfo, nullptr, &rp.renderPass));

    //? IMAGES
    const auto& extent = g_contextPtr->surfaceCapabilities.currentExtent;
    rp.msaaImage.Create(
        COLOR_FORMAT,
        extent.width, extent.height,
        rp.sampleCount,
        VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |  VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    );
    rp.msaaImage.Transition(cmdPool, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT);

    rp.renderImage.Create(
        COLOR_FORMAT,
        extent.width, extent.height,
        VK_SAMPLE_COUNT_1_BIT,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );

    rp.depthImage.Create(
        DEPTH_FORMAT,
        extent.width, extent.height,
        rp.sampleCount,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_IMAGE_ASPECT_DEPTH_BIT,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );
    rp.depthImage.Transition(cmdPool, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL, VK_IMAGE_ASPECT_DEPTH_BIT);

    //rp.depthImage.Create(cmdPool, DEPTH_FORMAT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, rp.sampleCount);

    const VkImageView views [] {
        rp.msaaImage.view,
        rp.depthImage.view,
        rp.renderImage.view,
    };

    //? FRAMEBUFFER
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