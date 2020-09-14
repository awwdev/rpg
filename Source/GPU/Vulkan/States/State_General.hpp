//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/States/General/General_RenderPass.hpp"
#include "GPU/Vulkan/States/General/General_Pipeline.hpp"
#include "GPU/Vulkan/States/General/General_Shader.hpp"
#include "GPU/Vulkan/States/General/General_VBO.hpp"
#include "GPU/Vulkan/States/General/General_UBO.hpp"

namespace rpg::gpu::vuk {

struct State_General
{
    General_Pipeline    pipeline;
    General_RenderPass  renderPass;
    General_Shader      shader;
    General_VBO         vbo;
    General_UBO         ubo;

    void Create(VkCommandPool cmdPool)
    {
        pipeline.Create();
        renderPass.Create(cmdPool);
        shader.Create();
        vbo.Create();
        ubo.Create();
    }

    void Clear()
    {
        pipeline.Clear();
        renderPass.Clear();
        shader.Clear();
        vbo.Clear();
        ubo.Clear();
    }

    void Update()
    {

    }

    void Record()
    {

    };
    
};

}//NS