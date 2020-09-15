//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/States/Post/Post_Pipeline.hpp"
#include "GPU/Vulkan/States/Post/Post_Shader.hpp"
#include "GPU/Vulkan/States/Post/Post_RenderPass.hpp"

namespace rpg::gpu::vuk {

struct State_Post
{
    Post_Pipeline   pipeline;
    Post_Shader     shader;
    Post_RenderPass renderPass;

    void Create(VkCommandPool cmdPool)
    {
        pipeline.Create();
        shader.Create();
        renderPass.Create();
    }

    void Clear()
    {
        pipeline.Clear();
        shader.Clear();
        renderPass.Clear();
    }

    void Update()
    {

    }

    void Record(VkCommandBuffer cmdBuffer)
    {

    };

    
};

}//NS