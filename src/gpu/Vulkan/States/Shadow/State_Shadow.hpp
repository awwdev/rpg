//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/States/Shadow/Shadow_RenderPass.hpp"
#include "gpu/Vulkan/States/Shadow/Shadow_Pipeline.hpp"
#include "gpu/Vulkan/States/Shadow/Shadow_Shader.hpp"
#include "gpu/Vulkan/States/Shadow/Shadow_Uniforms.hpp"
#include "gpu/Vulkan/States/Shadow/Shadow_Vertices.hpp"

#include "gpu/Vulkan/States/General/State_General.hpp"

#include "gpu/RenderData/RenderData.hpp"

namespace rpg::gpu::vuk {

struct State_Shadow
{
    Shadow_Pipeline   pipeline;
    Shadow_RenderPass renderPass;
    Shadow_Shader     shader;
    Shadow_Uniforms   uniforms;
    Shadow_Vertices   vertices;

    void Create(VkCommandPool cmdPool)
    {
        vertices    .Create();
        uniforms    .Create();
        shader      .Create();
        renderPass  .Create(cmdPool);
        pipeline    .Create(renderPass, shader, vertices, uniforms);
    }

    void Destroy()
    {
        pipeline    .Destroy();
        renderPass  .Destroy();
        shader      .Destroy();
        uniforms    .Destroy();
        vertices    .Destroy();
    }

    void Update(gpu::RenderData& renderData)
    {
        uniforms.Update(renderData.shadow.uboShadowMap);
    }

    void Record(VkCommandBuffer cmdBuffer, State_General& general)
    {
        for(uint32_t cascadeIdx = 0; cascadeIdx < RenderData_Shadow::CASCADE_COUNT; ++cascadeIdx)
        {
            uniforms.pushConst.data.cascadeIdx = cascadeIdx;

            vkCmdBeginRenderPass    (cmdBuffer, &renderPass.beginInfos[cascadeIdx], VK_SUBPASS_CONTENTS_INLINE);
            vkCmdPushConstants      (cmdBuffer, pipeline.layout, uniforms.pushConst.rangeInfo.stageFlags, uniforms.pushConst.rangeInfo.offset, 
                                     uniforms.pushConst.rangeInfo.size, &uniforms.pushConst.data);
            vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0, 
                                     uniforms.descriptors.descSets.count, uniforms.descriptors.descSets.data, 0, nullptr);
            vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
            vkCmdSetDepthBias       (cmdBuffer, -10, 0, -10);
            vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &general.vertices.vboTerrain.activeBuffer->buffer, general.vertices.offsets);
            vkCmdDraw               (cmdBuffer, general.vertices.vboTerrain.count, 1, 0, 0);
            vkCmdEndRenderPass      (cmdBuffer);
        }
    };
    
};

}//NS