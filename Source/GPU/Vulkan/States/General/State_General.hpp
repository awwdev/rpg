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
    General_RenderPass renderPass;

    void Create(VkCommandPool cmdPool)
    {
        //renderPass.Create(cmdPool);
    }

    void Clear()
    {
        //renderPass.Clear();
    }

    void Update(gpu::RenderData& )
    {

    }

    void Record(VkCommandBuffer cmdBuffer)
    {
        //vkCmdBeginRenderPass(cmdBuffer, &renderPass.beginInfo, VK_SUBPASS_CONTENTS_INLINE);
        //vkCmdEndRenderPass(cmdBuffer);
    };
    
};

}//NS