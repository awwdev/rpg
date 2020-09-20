//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/States/General/General_RenderPass.hpp"
#include "GPU/Vulkan/States/General/General_Pipeline.hpp"
#include "GPU/Vulkan/States/General/General_Shader.hpp"
#include "GPU/Vulkan/States/General/General_Vertices.hpp"
#include "GPU/RenderData.hpp"

namespace rpg::gpu::vuk {

struct State_General
{
    General_RenderPass  renderPass;
    General_Shader      shader;
    General_Pipeline    pipeline;
    General_Vertices    vertices;

    void Create(VkCommandPool cmdPool)
    {
        vertices    .Create();
        shader      .Create();
        renderPass  .Create(cmdPool);
        pipeline    .Create(renderPass, shader, vertices);
    }

    void Destroy()
    {
        pipeline    .Destroy();
        renderPass  .Destroy();
        shader      .Destroy();
        vertices    .Destroy();
    }

    void Update(const gpu::RenderData&)
    {

    }

    void Record(VkCommandBuffer cmdBuffer)
    {
        vkCmdBeginRenderPass    (cmdBuffer, &renderPass.beginInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
        vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &vertices.gpuVbo.activeBuffer->buffer, vertices.offsets);
        vkCmdDraw               (cmdBuffer, vertices.gpuVbo.count, 1, 0, 0);
        vkCmdEndRenderPass      (cmdBuffer);
    };
    
};

}//NS