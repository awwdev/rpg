//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Wrappers/RenderPass.hpp"

namespace rpg::gpu::vuk {

namespace rp_main
{
    enum ClearEnum
    {
        ColorClear = 0,
        DepthClear = 1,
        CLEAR_ENUM_END
    };

    enum ImageEnum
    {
        DepthImage = 0,
        MsaaImage  = 1,
        FinalImage = 2,
        IMAGE_ENUM_END
    };
}

inline void CreateRenderPass_Main(RenderPass& rp)
{
    using namespace rp_main;

    auto& width = rp.width  = g_contextPtr->surfaceCapabilities.currentExtent.width;
    auto& height = rp.height = g_contextPtr->surfaceCapabilities.currentExtent.height;
    auto& msaaSampleCount = rp.msaaSampleCount = VK_SAMPLE_COUNT_4_BIT;
    auto& colorFormat = rp.colorFormat = VK_FORMAT_R8G8B8A8_UNORM;
    auto& depthFormat = rp.depthFormat = VK_FORMAT_D32_SFLOAT;

    rp.clears.count = CLEAR_ENUM_END;
    rp.clears[ColorClear].color =  { 0, 0, 0, 1 };
    rp.clears[DepthClear].depthStencil = { 0, 0 };

    rp.images.count  = IMAGE_ENUM_END;
    auto& depthImage = rp.images[DepthImage];
    auto& msaaImage  = rp.images[MsaaImage];
    auto& finalImage = rp.images[FinalImage];

    //? MSAA
    msaaImage.Create(
        colorFormat,
        width, height,
        msaaSampleCount,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |  VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT
    );

    const VkAttachmentDescription msaaDesc {
        .flags          = 0 ,
        .format         = rp.colorFormat,
        .samples        = rp.msaaSampleCount,
        .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    };
    msaaImage.currentLayout = msaaDesc.finalLayout;

    const VkAttachmentReference msaaRef {
        .attachment = 0,
        .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    //? DEPTH
    depthImage.Create(
        depthFormat,
        width, height,
        msaaSampleCount,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_IMAGE_ASPECT_DEPTH_BIT
    );

    const VkAttachmentDescription depthDesc {
        .flags          = 0 ,
        .format         = depthFormat, 
        .samples        = msaaSampleCount,
        .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,        
    };
    depthImage.currentLayout = depthDesc.finalLayout;

    const VkAttachmentReference depthRef {
        .attachment = 1,
        .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };

    //? FINAL
    finalImage.Create(
        colorFormat,
        width, height,
        VK_SAMPLE_COUNT_1_BIT,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT
    );

    const VkAttachmentDescription finalDesc {
        .flags          = 0 ,
        .format         = colorFormat, 
        .samples        = VK_SAMPLE_COUNT_1_BIT,
        .loadOp         = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,        
    };
    finalImage.currentLayout = finalDesc.finalLayout;

    const VkAttachmentReference finalRef {
        .attachment = 2,
        .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    //? SUMMARY
    const VkAttachmentDescription descs [] {
        msaaDesc,
        depthDesc,
        finalDesc,
    };

    const VkImageView views [] {
        msaaImage.view,
        depthImage.view,
        finalImage.view,
    };

    //? SUBPASS
    const VkSubpassDescription subpassDesc {
        .flags                   = 0,
        .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .inputAttachmentCount    = 0,
        .pInputAttachments       = nullptr,
        .colorAttachmentCount    = 1,
        .pColorAttachments       = &msaaRef,
        .pResolveAttachments     = &finalRef,
        .pDepthStencilAttachment = &depthRef,
        .preserveAttachmentCount = 0,
        .pPreserveAttachments    = nullptr
    };

    VkSubpassDependency const dependencies []
    {
        {
            .srcSubpass         = VK_SUBPASS_EXTERNAL,
            .dstSubpass         = 0,
            .srcStageMask       = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            .dstStageMask       = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask      = VK_ACCESS_MEMORY_READ_BIT,
            .dstAccessMask      = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .dependencyFlags    = VK_DEPENDENCY_BY_REGION_BIT,
        },
        {
            .srcSubpass         = 0,
            .dstSubpass         = VK_SUBPASS_EXTERNAL,
            .srcStageMask       = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask       = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            .srcAccessMask      = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .dstAccessMask      = VK_ACCESS_MEMORY_READ_BIT,
            .dependencyFlags    = VK_DEPENDENCY_BY_REGION_BIT,
        }
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
    VkCheck(vkCreateRenderPass(g_contextPtr->device, &renderPassInfo, nullptr, &rp.renderPass));

    //? FRAMEBUFFER
    const VkFramebufferCreateInfo framebufferInfo{
        .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .pNext           = nullptr,
        .flags           = 0,
        .renderPass      = rp.renderPass,
        .attachmentCount = array_extent(views),
        .pAttachments    = views,
        .width           = width,
        .height          = height,
        .layers          = 1
    };
    rp.framebuffers.Append();
    VkCheck(vkCreateFramebuffer(g_contextPtr->device, &framebufferInfo, nullptr, &rp.framebuffers[0]));

    //? BEGIN INFO
    rp.beginInfos.Append();
    rp.beginInfos[0] = {
        .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext          = nullptr, 
        .renderPass     = rp.renderPass,
        .framebuffer    = rp.framebuffers[0],
        .renderArea     = 
        {
            .offset     = {},
            .extent     = { width, height }
        },
        .clearValueCount= rp.clears.count,
        .pClearValues   = rp.clears.data
    };    

}

}//ns