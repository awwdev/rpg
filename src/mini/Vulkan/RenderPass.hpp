#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"


namespace mini::vk
{
    struct RenderPass
    {
        VkRenderPass renderPass;
        //framebuffer

        inline void Destroy(VkDevice device)
        {
            vkDestroyRenderPass(device, renderPass, nullptr);
        }
    };

    inline RenderPass CreateRenderPass_Default(Context& context)
    {
        RenderPass renderPass;
        
        //attachments
        //VkAttachmentDescription
        //reference
        //VK_CHECK(vkCreateRenderPass(context.device, &renderInfo, nullptr, &renderPass));
        //framebuffer ot view

        return renderPass;
    }

}//ns