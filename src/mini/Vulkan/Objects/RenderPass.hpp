//https://github.com/awwdev

#pragma once
#include "mini/Vulkan/Core.hpp"
#include "mini/Vulkan/Context.hpp"

#include "mini/Box/Array.hpp"

namespace mini::vk
{
    struct RenderPass
    {
        VkRenderPass renderPass;
        box::Array<VkFramebuffer, 4> framebuffers;
        const VkSampleCountFlagBits SAMPLE_COUNT = VK_SAMPLE_COUNT_1_BIT;

        ~RenderPass()
        {
            vkDestroyRenderPass(context.device, renderPass, nullptr);
            FOR_SIMPLE_ARRAY(framebuffers, i) 
                vkDestroyFramebuffer(context.device, framebuffers[i], nullptr);
        }
        
    };

}//ns