//https://github.com/awwdev

#pragma once

#include "gpu/Vulkan/Context.hpp"
#include "gpu/Vulkan/Wrappers/Image.hpp"

namespace rpg::gpu::vuk {

struct Main_RenderPass
{
    //? DATA
    static constexpr auto DEPTH_FORMAT = VK_FORMAT_D32_SFLOAT;
    static constexpr auto COLOR_FORMAT = VK_FORMAT_R8G8B8A8_UNORM;

    VkRenderPassBeginInfo beginInfo;
    VkFramebuffer framebuffer; //offscreen (1x)
    VkRenderPass  renderPass;

    Image depthImage;
    Image msaaImage;
    Image finalImage;

    uint32_t width, height; //used by pipeline for view, scissor
    VkSampleCountFlagBits msaaSampleCount = VK_SAMPLE_COUNT_4_BIT;

    VkClearValue clears [2] { 
        { .color        = { 0, 0, 0, 1 } },
        { .depthStencil = { 0, 0 } } //reversed z        
    };


    //? CREATE
    void Create(VkCommandPool cmdPool)
    {
        width  = g_contextPtr->surfaceCapabilities.currentExtent.width;
        height = g_contextPtr->surfaceCapabilities.currentExtent.height;

        //? MSAA
        msaaImage.Create(
            COLOR_FORMAT,
            width, height,
            msaaSampleCount,
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |  VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
            VK_IMAGE_ASPECT_COLOR_BIT
        );

        const VkAttachmentDescription msaaDesc {
            .flags          = 0 ,
            .format         = COLOR_FORMAT, 
            .samples        = msaaSampleCount,
            .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        };
        msaaImage.currentLayout = msaaDesc.finalLayout; //VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL

        const VkAttachmentReference msaaRef {
            .attachment = 0,
            .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

         //? DEPTH
        depthImage.Create(
            DEPTH_FORMAT,
            width, height,
            msaaSampleCount,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VK_IMAGE_ASPECT_DEPTH_BIT
        );

        const VkAttachmentDescription depthDesc {
            .flags          = 0 ,
            .format         = DEPTH_FORMAT, 
            .samples        = msaaSampleCount,
            .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,        
        };
        depthImage.currentLayout = depthDesc.finalLayout; //VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL

        const VkAttachmentReference depthRef {
            .attachment = 1,
            .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        };

        //? FINAL
        finalImage.Create(
            COLOR_FORMAT,
            width,height,
            VK_SAMPLE_COUNT_1_BIT,
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_IMAGE_ASPECT_COLOR_BIT
        );

        const VkAttachmentDescription finalDesc {
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

        /* vk tutorial
        const VkSubpassDependency dependencies []
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
        */

        //sascha willems
        //https://github.com/SaschaWillems/Vulkan/blob/c13a715ead325b8470e9511a588b001644e02568/base/vulkanexamplebase.cpp
        const VkSubpassDependency dependencies []
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
        VkCheck(vkCreateRenderPass(g_contextPtr->device, &renderPassInfo, nullptr, &renderPass));

        //? FRAMEBUFFER
        const VkFramebufferCreateInfo framebufferInfo{
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
        VkCheck(vkCreateFramebuffer(g_contextPtr->device, &framebufferInfo, nullptr, &framebuffer));

        //? BEGIN INFO
        beginInfo = {
            .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext          = nullptr, 
            .renderPass     = renderPass,
            .framebuffer    = framebuffer,
            .renderArea     = 
            {
                .offset     = {},
                .extent     = { width, height }
            },
            .clearValueCount= (uint32_t) array_extent(clears),
            .pClearValues   = clears
        };
    }
    
    //? RAII
    ~Main_RenderPass() 
    { 
        Destroy(); 
    }

    void Destroy()
    {
        vkDestroyRenderPass (g_contextPtr->device, renderPass, nullptr);
        vkDestroyFramebuffer(g_contextPtr->device, framebuffer, nullptr);
        finalImage.Destroy();    
        depthImage.Destroy();    
        msaaImage.Destroy();
    }

};

}//ns