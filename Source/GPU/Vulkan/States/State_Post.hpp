//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/States/Post/Post_Pipeline.hpp"
#include "GPU/Vulkan/States/Post/Post_Shader.hpp"
#include "GPU/Vulkan/States/Post/Post_RenderPass.hpp"
#include "GPU/Vulkan/States/Post/Post_VBO.hpp"

namespace rpg::gpu::vuk {

struct State_Post
{
    Post_Pipeline   pipeline;
    Post_Shader     shader;
    Post_RenderPass renderPass;
    Post_VBO        vbo;

    void Create(VkCommandPool cmdPool)
    {
        pipeline.Create();
        shader.Create();
        renderPass.Create();
        vbo.Create();
    }

    void Clear()
    {
        pipeline.Clear();
        shader.Clear();
        renderPass.Clear();
        vbo.Clear();
    }

    void Update()
    {

    }

    void Record()
    {

    };

    
};

}//NS