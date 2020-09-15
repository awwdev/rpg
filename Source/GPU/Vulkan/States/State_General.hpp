//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/States/General/General_RenderPass.hpp"
#include "GPU/Vulkan/States/General/General_Pipeline.hpp"
#include "GPU/Vulkan/States/General/General_Shader.hpp"

#include "GPU/Vulkan/Objects/VertexBuffer.hpp"

namespace rpg::gpu::vuk {

struct Vertex_General{};

struct State_General
{
    General_Pipeline    pipeline;
    General_RenderPass  renderPass;
    General_Shader      shader;
    
    VertexBuffer<Vertex_General, 10> vbo;

    void Create(VkCommandPool cmdPool)
    {
        pipeline.Create(renderPass, vbo);
        renderPass.Create(cmdPool);
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