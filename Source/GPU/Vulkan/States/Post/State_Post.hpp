//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/States/Post/Post_Pipeline.hpp"
#include "GPU/Vulkan/States/Post/Post_Shader.hpp"
#include "GPU/Vulkan/States/Post/Post_RenderPass.hpp"
#include "GPU/Vulkan/States/Post/Post_Uniforms.hpp"
#include "GPU/Vulkan/States/Post/Post_Vertices.hpp"
#include "GPU/RenderData.hpp"

#include "GPU/Vulkan/States/General/State_General.hpp"

namespace rpg::gpu::vuk {

struct State_Post
{
    Post_RenderPass renderPass;
    Post_Shader     shader;
    Post_Pipeline   pipeline;
    Post_Uniforms   uniforms;
    Post_Vertices   vertices;

    void Create(VkCommandPool cmdPool, State_General& general)
    {
        uniforms    .Create(general.renderPass.finalImage);
        vertices    .Create();
        shader      .Create();
        renderPass  .Create();
        pipeline    .Create(renderPass, shader, uniforms);
    }

    void Clear()
    {
        pipeline    .Clear();
        renderPass  .Clear();
        shader      .Clear();
        uniforms    .Clear();
        vertices    .Clear();
    }

    void Update(gpu::RenderData&)
    {

    }

    void Record(VkCommandBuffer cmdBuffer, const uint32_t cmdBufferIdx)
    {
        vkCmdBeginRenderPass    (cmdBuffer, &renderPass.beginInfos[cmdBufferIdx], VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
        vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0, 
                                 uniforms.descriptors.descSets.count, uniforms.descriptors.descSets.data, 0, nullptr);
        vkCmdDraw               (cmdBuffer, 3, 1, 0, 0);
        vkCmdEndRenderPass      (cmdBuffer);
    };
    
};

}//NS