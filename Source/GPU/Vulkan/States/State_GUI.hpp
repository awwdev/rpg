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
    GUI_Pipeline       pipeline;
    GUI_Shader         shader;
    GUI_RenderPass     renderPass;
    GUI_Uniforms       uniforms;

    void Create(res::HostResources& hostRes, VkCommandPool cmdPool)
    {
        uniforms.Create(hostRes, cmdPool);
        shader.Create();
        renderPass.Create();
        pipeline.Create(renderPass, shader, uniforms);
    }

    void Clear()
    {
        uniforms.Clear();
        pipeline.Clear();
        renderPass.Clear();
        shader.Clear();
    }

    void Update(gpu::RenderData& renderData)
    {
        uniforms.ubo.Reset();
        uniforms.ubo.Store(renderData.gui_ubo);
    }

    void Record(VkCommandBuffer cmdBuffer, const uint32_t cmdBufferIdx)
    {
        const auto beginInfo = renderPass.GetBeginInfo(cmdBufferIdx);
        
        vkCmdBeginRenderPass    (cmdBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
        if (uniforms.ubo.count > 0)
        {
            vkCmdBindPipeline       (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
            vkCmdBindDescriptorSets (cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0, 
            uniforms.descriptors.descSets.count, uniforms.descriptors.descSets.data, 0, nullptr);
            vkCmdDraw(cmdBuffer, uniforms.ubo.count * 6, 1, 0, 0); 
        }
        vkCmdEndRenderPass      (cmdBuffer);
    };

};

}//NS