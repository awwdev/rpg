//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/States/General/General_RenderPass.hpp"
#include "GPU/Vulkan/States/General/General_Pipeline.hpp"
#include "GPU/Vulkan/States/General/General_Shader.hpp"
#include "GPU/Vulkan/_Old/Objects/VertexBuffer.hpp"
#include "GPU/RenderData.hpp"

namespace rpg::gpu::vuk {

struct State_General
{
    General_RenderPass  renderPass;
    General_Shader      shader;
    General_Pipeline    pipeline;

    void Create(VkCommandPool cmdPool)
    {
        shader      .Create();
        renderPass  .Create(cmdPool);
        pipeline    .Create(renderPass, shader);
    }

    void Destroy()
    {
        pipeline    .Destroy();
        renderPass  .Destroy();
        shader      .Destroy();
    }

    void Update(const gpu::RenderData&)
    {

    }

    void Record(VkCommandBuffer cmdBuffer)
    {
        vkCmdBeginRenderPass    (cmdBuffer, &renderPass.beginInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
        vkCmdDraw               (cmdBuffer, 3, 1, 0, 0);
        vkCmdEndRenderPass      (cmdBuffer);
    };
    
};

}//NS