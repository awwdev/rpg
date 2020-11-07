//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Passes/GUI/GUI_Pipeline.hpp"
#include "gpu/Vulkan/Passes/GUI/GUI_Shader.hpp"
#include "gpu/Vulkan/Passes/GUI/GUI_RenderPass.hpp"
#include "gpu/Vulkan/Passes/GUI/GUI_Uniforms.hpp"
#include "gpu/RenderData/RenderData.hpp"
#include "res/Resources.hpp"

namespace rpg::gpu::vuk {

struct Pass_GUI
{
    GUI_Pipeline    pipeline;
    GUI_Shader      shader;
    GUI_RenderPass  renderPass;
    GUI_Uniforms    uniforms;

    void Create(VkCommandPool cmdPool, res::Resources& resources)
    {
        uniforms    .Create(cmdPool, resources);
        shader      .Create();
        renderPass  .Create();
        pipeline    .Create(renderPass, shader, uniforms);
    }

    void Destroy()
    {
        uniforms    .Destroy();
        pipeline    .Destroy();
        renderPass  .Destroy();
        shader      .Destroy();
    }

    void Update(const gpu::RenderData& renderData)
    {
        uniforms.Update(renderData.gui.uboText);
    }

    void Record(VkCommandBuffer cmdBuffer, const uint32_t cmdBufferIdx)
    {
        if (uniforms.uboText.count == 0){
            vkCmdBeginRenderPass(cmdBuffer, &renderPass.beginInfos[cmdBufferIdx], VK_SUBPASS_CONTENTS_INLINE);
            vkCmdEndRenderPass(cmdBuffer);
            return;
        }

        vkCmdBeginRenderPass    (cmdBuffer, &renderPass.beginInfos[cmdBufferIdx], VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
        vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0, 
                                 uniforms.descriptors.descSets.count, uniforms.descriptors.descSets.data, 0, nullptr);
        vkCmdDraw               (cmdBuffer, uniforms.uboText.count * 6, 1, 0, 0);
        vkCmdEndRenderPass      (cmdBuffer);
    };

};

}//NS