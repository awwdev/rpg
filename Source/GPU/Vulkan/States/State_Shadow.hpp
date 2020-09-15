//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/States/Shadow/Shadow_RenderPass.hpp"
#include "GPU/Vulkan/States/Shadow/Shadow_Pipeline.hpp"
#include "GPU/Vulkan/States/Shadow/Shadow_Shader.hpp"

namespace rpg::gpu::vuk {

struct State_Shadow
{
    Shadow_Pipeline   pipeline;
    Shadow_RenderPass renderPass;
    Shadow_Shader     shader;

    void Create(VkCommandPool cmdPool)
    {
        pipeline.Create();
        renderPass.Create();
        shader.Create();
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

    void Record(VkCommandBuffer cmdBuffer)
    {

    };
    
};

}//NS