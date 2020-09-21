//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/States/Shadow/Shadow_RenderPass.hpp"
#include "GPU/Vulkan/States/Shadow/Shadow_Pipeline.hpp"
#include "GPU/Vulkan/States/Shadow/Shadow_Shader.hpp"
#include "GPU/Vulkan/States/Shadow/Shadow_Uniforms.hpp"
#include "GPU/Vulkan/Objects/PushConstant.hpp"
#include "GPU/RenderData.hpp"
#include "GPU/RenderStructs.hpp"

#include "GPU/Vulkan/States/General/State_General.hpp"

namespace rpg::gpu::vuk {

struct State_Shadow
{
    Shadow_Pipeline   pipeline;
    Shadow_RenderPass renderPass;
    Shadow_Shader     shader;
    Shadow_Uniforms   uniforms;

    void Create(VkCommandPool cmdPool, State_General& general)
    {
        uniforms    .Create();
        shader      .Create();
        renderPass  .Create(cmdPool);
        pipeline    .Create(renderPass, shader, general.vertices, uniforms);
    }

    void Destroy()
    {
        pipeline    .Destroy();
        renderPass  .Destroy();
        shader      .Destroy();
        uniforms    .Destroy();
    }

    void Update(gpu::RenderData& renderData)
    {
        uniforms.Update(renderData.uboData_shadow_sun);
    }

    void Record(VkCommandBuffer cmdBuffer, State_General& general)
    {
        for(uint32_t cascadeIdx = 0; cascadeIdx < gpu::CASCADE_COUNT; ++cascadeIdx)
        {
            uniforms.pushConst.data.cascadeIdx = cascadeIdx;

            vkCmdBeginRenderPass    (cmdBuffer, &renderPass.beginInfos[cascadeIdx], VK_SUBPASS_CONTENTS_INLINE);
            vkCmdPushConstants      (cmdBuffer, pipeline.layout, uniforms.pushConst.rangeInfo.stageFlags, uniforms.pushConst.rangeInfo.offset, 
                                     uniforms.pushConst.rangeInfo.size, &uniforms.pushConst.data);
            vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0, 
                                     uniforms.descriptors.descSets.count, uniforms.descriptors.descSets.data, 0, nullptr);
            vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
            vkCmdBindVertexBuffers  (cmdBuffer, 0, 1, &general.vertices.gpuVbo.activeBuffer->buffer, general.vertices.offsets);
            vkCmdDraw               (cmdBuffer, general.vertices.gpuVbo.count, 1, 0, 0);
            vkCmdEndRenderPass      (cmdBuffer);
        }
    };
    
};

}//NS