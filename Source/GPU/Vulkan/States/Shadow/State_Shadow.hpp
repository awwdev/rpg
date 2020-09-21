//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/States/Shadow/Shadow_RenderPass.hpp"
#include "GPU/Vulkan/States/Shadow/Shadow_Pipeline.hpp"
#include "GPU/Vulkan/States/Shadow/Shadow_Shader.hpp"
#include "GPU/RenderData.hpp"

namespace rpg::gpu::vuk {

struct State_Shadow
{
    Shadow_Pipeline   pipeline;
    Shadow_RenderPass renderPass;
    Shadow_Shader     shader;

    void Create(VkCommandPool cmdPool)
    {
        shader      .Create();
        renderPass  .Create(cmdPool);
        pipeline    .Create();
    }

    void Destroy()
    {
        pipeline    .Destroy();
        renderPass  .Destroy();
        shader      .Destroy();
    }

    void Update(gpu::RenderData&)
    {

    }

    void Record(VkCommandBuffer cmdBuffer)
    {
        for(uint32_t cascadeIdx = 0; cascadeIdx < renderPass.CASCADE_COUNT; ++cascadeIdx)
        {
            vkCmdBeginRenderPass    (cmdBuffer, &renderPass.beginInfos[cascadeIdx], VK_SUBPASS_CONTENTS_INLINE);
            vkCmdEndRenderPass      (cmdBuffer);
        }
    };
    
};

}//NS