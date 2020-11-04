//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/States/Post/Post_Pipeline.hpp"
#include "gpu/Vulkan/States/Post/Post_Shader.hpp"
#include "gpu/Vulkan/States/Post/Post_RenderPass.hpp"
#include "gpu/Vulkan/States/Post/Post_Uniforms.hpp"
#include "gpu/Vulkan/States/Post/Post_Vertices.hpp"
#include "gpu/RenderData/RenderData.hpp"

namespace rpg::gpu::vuk {

struct State_Post
{
    Post_RenderPass renderPass;
    Post_Shader     shader;
    Post_Pipeline   pipeline;
    Post_Uniforms   uniforms;
    Post_Vertices   vertices;

    void Create(VkCommandPool cmdPool, Image& finalImage)
    {
        uniforms    .Create(finalImage);
        vertices    .Create();
        shader      .Create();
        renderPass  .Create();
        pipeline    .Create(renderPass, shader, uniforms, vertices);
    }

    void Destroy()
    {
        pipeline    .Destroy();
        renderPass  .Destroy();
        shader      .Destroy();
        uniforms    .Destroy();
        vertices    .Destroy();
    }

    void Update(const gpu::RenderData& renderData)
    {
        vertices.Update(renderData.post.vboBlur);
    }

    void Record(VkCommandBuffer cmdBuffer, const uint32_t cmdBufferIdx)
    {
        vkCmdBeginRenderPass    (cmdBuffer, &renderPass.beginInfos[cmdBufferIdx], VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
        vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0, 
                                 uniforms.descriptors.descSets.count, uniforms.descriptors.descSets.data, 0, nullptr);
        vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &vertices.gpuVbo.activeBuffer->buffer, vertices.offsets);
        vkCmdDraw               (cmdBuffer, vertices.gpuVbo.count, 1, 0, 0);
        vkCmdEndRenderPass      (cmdBuffer);
    };
    
};

}//NS