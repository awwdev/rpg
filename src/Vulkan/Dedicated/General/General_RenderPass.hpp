//https://github.com/awwdev

#pragma once
#include "Vulkan/Core.hpp"
#include "Vulkan/Context.hpp"
#include "Vulkan/Objects/Image.hpp"

namespace rpg::vuk {

struct General_RenderPass
{
    //? DATA
    constexpr auto DEPTH_FORMAT = VK_FORMAT_D32_SFLOAT;
    constexpr auto COLOR_FORMAT = VK_FORMAT_R8G8B8A8_UNORM;
    constexpr auto MSAA_SAMPLE_COUNT = VK_SAMPLE_COUNT_4_BIT;

    VkRenderPass  renderPass;
    VkFramebuffer framebuffer; //offscreen

    Image depthImage;
    Image msaaImage;
    Image finalImage;

    //? RAII
    ~RenderPassTest() 
    { 
        Clear(); 
    }

    void Clear()
    {
        vkDestroyRenderPass (g_contextPtr->device, renderPass, nullptr);
        vkDestroyFramebuffer(g_contextPtr->device, framebuffer, nullptr);
        finalImage.Clear();    
        depthImage.Clear();    
        msaaImage.Clear();
    }

    //? HELPER
    template<idx_t N>
    VkRenderPassBeginInfo GetBeginInfo(const VkClearValue (&clears) [N])
    {
        return {
            .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext          = nullptr, 
            .renderPass     = renderPass,
            .framebuffer    = framebuffer,
            .renderArea     = 
            {
                .offset     = {},
                .extent     = g_contextPtr->surfaceCapabilities.currentExtent,
            },
            .clearValueCount= N,
            .pClearValues   = clears
        };
    }

    //? CREATE
    void Create(VkCommandPool cmdPool)
    {
        const auto& extent = g_contextPtr->surfaceCapabilities.currentExtent;

        //? MSAA
        msaaImage.Create(
            COLOR_FORMAT,
            extent.width, extent.height,
            sampleCount,
            VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |  VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
            VK_IMAGE_ASPECT_COLOR_BIT,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        );

        const VkAttachmentDescription msaaDesc {
            .flags          = 0 ,
            .format         = COLOR_FORMAT, 
            .samples        = MSAA_SAMPLE_COUNT,
            .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        };

        const VkAttachmentReference msaaRef {
            .attachment = 0,
            .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

         //? DEPTH
        depthImage.Create(
            DEPTH_FORMAT,
            extent.width, extent.height,
            sampleCount,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VK_IMAGE_ASPECT_DEPTH_BIT,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );

        const VkAttachmentDescription depthDesc {
            .flags          = 0 ,
            .format         = DEPTH_FORMAT, 
            .samples        = MSAA_SAMPLE_COUNT,
            .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,        
        };

        const VkAttachmentReference depthRef {
            .attachment = 1,
            .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        };

        //? FINAL
        finalImage.Create(
            COLOR_FORMAT,
            extent.width, extent.height,
            VK_SAMPLE_COUNT_1_BIT,
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_IMAGE_ASPECT_COLOR_BIT,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
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
            .pColorAttachments       = &colorRef,
            .pResolveAttachments     = &resolveRef,
            .pDepthStencilAttachment = &depthRef,
            .preserveAttachmentCount = 0,
            .pPreserveAttachments    = nullptr
        };

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
        VkCheck(vkCreateRenderPass(g_contextPtr->device, &renderPassInfo, nullptr, &renderPass));

        //? FRAMEBUFFER
        const VkFramebufferCreateInfo framebufferInfo{
            .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext           = nullptr,
            .flags           = 0,
            .renderPass      = renderPass,
            .attachmentCount = ArrayCount(views),
            .pAttachments    = views,
            .width           = extent.width,
            .height          = extent.height,
            .layers          = 1
        };
        VkCheck(vkCreateFramebuffer(g_contextPtr->device, &framebufferInfo, nullptr, &framebuffer));
    }
    
};

}//ns