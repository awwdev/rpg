//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/States/GUI/GUI_Pipeline.hpp"
#include "GPU/Vulkan/States/GUI/GUI_Shader.hpp"
#include "GPU/Vulkan/States/GUI/GUI_RenderPass.hpp"
#include "GPU/Vulkan/States/GUI/GUI_Uniforms.hpp"
#include "Resources/CpuResources.hpp"
#include "GPU/RenderData.hpp"

namespace rpg::gpu::vuk {

struct State_GUI
{
    GUI_Pipeline    pipeline;
    GUI_Shader      shader;
    GUI_RenderPass  renderPass;
    GUI_Uniforms    uniforms;

    void Create(VkCommandPool cmdPool, res::HostResources& hostRes)
    {
        uniforms    .Create(cmdPool, hostRes);
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
        uniforms.uboText.Reset();
        uniforms.uboText.Append(renderData.uboData_gui_text.Data(), renderData.uboData_gui_text.count);
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