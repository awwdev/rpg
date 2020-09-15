//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/States/GUI/GUI_Pipeline.hpp"
#include "GPU/Vulkan/States/GUI/GUI_Shader.hpp"
#include "GPU/Vulkan/States/GUI/GUI_RenderPass.hpp"

namespace rpg::gpu::vuk {

struct State_GUI
{
    GUI_Pipeline    pipeline;
    GUI_Shader      shader;
    GUI_RenderPass  renderPass;

    void Create(VkCommandPool cmdPool)
    {
        shader.Create();
        renderPass.Create();
        pipeline.Create(renderPass, shader);
    }

    void Clear()
    {
        pipeline.Clear();
        renderPass.Clear();
        shader.Clear();
    }

    void Update()
    {

    }

    void Record(VkCommandBuffer cmdBuffer, const uint32_t cmdBufferIdx)
    {
        const auto beginInfo = renderPass.GetBeginInfo(cmdBufferIdx);
        
        vkCmdBeginRenderPass    (cmdBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdEndRenderPass      (cmdBuffer);
    };

};

}//NS