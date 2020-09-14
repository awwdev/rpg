//https://github.com/awwdev

#pragma once

#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Objects/RenderPass.hpp"

namespace rpg::gpu::vuk {

inline void Shadow_CreateRenderPass(RenderPassDepth& rp, VkCommandPool cmdPool)
{  
    rp.sampleCount = VK_SAMPLE_COUNT_1_BIT;
    constexpr VkFormat DEPTH_FORMAT = VK_FORMAT_D32_SFLOAT;
    rp.width  = 2048*2;
    rp.height = 2048*2;
    rp.layerCount = 3;

    rp.depthImageArray.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    rp.depthImageArray.Create(
        cmdPool, 
        DEPTH_FORMAT, 
        rp.layerCount,
        rp.width, 
        rp.height,
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
        .attachmentCount = ArrayCount(descs),
        .pAttachments    = descs,
        .subpassCount    = 1,
        .pSubpasses      = &subpassDesc,
        .dependencyCount = ArrayCount(dependencies),
        .pDependencies   = dependencies
    };

    VkCheck(vkCreateRenderPass(g_contextPtr->device, &renderPassInfo, nullptr, &rp.renderPass));


    //? IMAGE AND FRAMEBUFFER PER LAYER (cascade)
    for(uint32_t i = 0; i < rp.layerCount; ++i) {

        const VkImageViewCreateInfo viewInfo 
        {
            .sType              = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext              = nullptr,
            .flags              = 0, 
            .image              = rp.depthImageArray.image, 
            .viewType           = VK_IMAGE_VIEW_TYPE_2D_ARRAY, //!
            .format             = DEPTH_FORMAT,
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
                .baseArrayLayer = i, //!
                .layerCount     = 1  //!
            }
        };
        auto view = rp.imageViews.Append();
        VkCheck(vkCreateImageView(g_contextPtr->device, &viewInfo, nullptr, view));

        const VkFramebufferCreateInfo framebufferInfo {
            .sType              = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,    
            .pNext              = 0,    
            .flags              = 0,    
            .renderPass         = rp.renderPass,        
            .attachmentCount    = 1,                
            .pAttachments       = view,            
            .width              = rp.width,    
            .height             = rp.height,    
            .layers             = 1,    
        };
        auto framebuffer = rp.framebuffers.Append();
        VkCheck(vkCreateFramebuffer(g_contextPtr->device, &framebufferInfo, nullptr, framebuffer));

    }
}

}//ns