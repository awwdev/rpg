//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Wrappers/RenderPass.hpp"
#include "gpu/RenderData/RenderData.hpp"

namespace rpg::gpu::vuk {

inline void CreateRenderPass_Shadow(RenderPass& rp)
{
    auto const& width  = rp.width  = RenderData_Shadow::SHADOW_MAP_SIZE;
    auto const& height = rp.height = RenderData_Shadow::SHADOW_MAP_SIZE; 
    auto const& depthFormat = rp.depthFormat = VK_FORMAT_D32_SFLOAT; 

    rp.clears.Append();
    rp.clears[0].depthStencil = { 0, 0 };

    //? DEPTH IMAGE ARRAY
    rp.images.count = 1;
    auto& shadowMaps = rp.images[0];
    shadowMaps.Create(
        depthFormat,
        width, height,
        VK_SAMPLE_COUNT_1_BIT,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_DEPTH_BIT,
        RenderData_Shadow::CASCADE_COUNT,
        VK_IMAGE_VIEW_TYPE_2D_ARRAY
    );

    //? ATTACHEMENT
    const VkAttachmentDescription depthDesc {
        .flags          = 0 ,
        .format         = depthFormat, 
        .samples        = VK_SAMPLE_COUNT_1_BIT,
        .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
    };
    shadowMaps.currentLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

    const VkAttachmentDescription descs [] {
        depthDesc,
    };
    const VkAttachmentReference depthRef {
        .attachment = 0,
        .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };

    //? SUBPASS
    const VkSubpassDescription subpassDesc {
        .flags                   = 0,
        .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .inputAttachmentCount    = 0,
        .pInputAttachments       = nullptr,
        .colorAttachmentCount    = 0, //!
        .pColorAttachments       = nullptr,
        .pResolveAttachments     = nullptr,
        .pDepthStencilAttachment = &depthRef,
        .preserveAttachmentCount = 0,
        .pPreserveAttachments    = nullptr
    };

    //https://github.com/SaschaWillems/Vulkan/blob/master/examples/shadowmapping/shadowmapping.cpp
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

     //? PER CASCADE
    for(uint32_t cascadeIdx = 0; cascadeIdx < RenderData_Shadow::CASCADE_COUNT; ++cascadeIdx)
    {
        //? VIEW
        const VkImageViewCreateInfo viewInfo 
        {
            .sType              = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext              = nullptr,
            .flags              = 0, 
            .image              = shadowMaps.image, 
            .viewType           = VK_IMAGE_VIEW_TYPE_2D_ARRAY, //!
            .format             = depthFormat,
            .components         = 
            {
                .r = VK_COMPONENT_SWIZZLE_R,
                .g = VK_COMPONENT_SWIZZLE_G,
                .b = VK_COMPONENT_SWIZZLE_B,
                .a = VK_COMPONENT_SWIZZLE_A
            },
            .subresourceRange   = 
            {
                .aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = cascadeIdx, //!
                .layerCount     = 1 //!
            }
        };
        rp.views.Append();
        VkCheck(vkCreateImageView(g_contextPtr->device, &viewInfo, nullptr, &rp.views[cascadeIdx]));

        //? FRAMEBUFFER
        const VkFramebufferCreateInfo framebufferInfo {
            .sType              = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,    
            .pNext              = 0,    
            .flags              = 0,    
            .renderPass         = rp.renderPass,        
            .attachmentCount    = 1,                
            .pAttachments       = &rp.views[cascadeIdx],            
            .width              = width,    
            .height             = height,    
            .layers             = 1,    
        };
        rp.framebuffers.Append();
        VkCheck(vkCreateFramebuffer(g_contextPtr->device, &framebufferInfo, nullptr, &rp.framebuffers[cascadeIdx]));

        //? BEGIN INFO
        rp.beginInfos.Append();
        rp.beginInfos[cascadeIdx] = {
            .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext          = nullptr, 
            .renderPass     = rp.renderPass,
            .framebuffer    = rp.framebuffers[cascadeIdx],
            .renderArea     = {
                .offset     = VkOffset2D {0, 0},
                .extent     = { width, height }
            },
            .clearValueCount= rp.clears.count,
            .pClearValues   = rp.clears.data
        };
    }

}

}//ns