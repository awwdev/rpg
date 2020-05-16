#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"


namespace mini::vk
{
    //custom render passes? since we have different setups (images)

    struct RenderPass
    {
        VkDevice device;

        VkRenderPass renderPass;
        VkArray<VkFramebuffer, 4> framebuffers { {}, 0 }; // count unknown at compile time (capacity)
        
        VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT;
        VkFormat colorFormat = VK_FORMAT_B8G8R8A8_UNORM;


        explicit RenderPass(Context& context) : device { context.device }
        {
            const VkAttachmentDescription colorDesc {
                .flags          = 0 ,
                .format         = colorFormat, 
                .samples        = sampleCount,
                .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
                .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
                .finalLayout    = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
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

            const VkRenderPassCreateInfo renderPassInfo {
                .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
                .pNext           = nullptr,
                .flags           = 0,
                .attachmentCount = 1,
                .pAttachments    = &colorDesc,
                .subpassCount    = 1,
                .pSubpasses      = &subpassDesc,
                .dependencyCount = 0,
                .pDependencies   = nullptr
            };

            VK_CHECK(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass));
        }

        ~RenderPass()
        {
            vkDestroyRenderPass(device, renderPass, nullptr);
            FOR_VK_ARRAY(framebuffers, i) 
                vkDestroyFramebuffer(device, framebuffers[i], nullptr);
        }
    };

}//ns