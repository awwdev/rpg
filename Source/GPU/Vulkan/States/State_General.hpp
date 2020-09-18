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
    General_RenderPass  renderPass;


    void Create(VkCommandPool cmdPool)
    {

    }

    void Clear()
    {

    }

    void Update(gpu::RenderData& )
    {

    }

    void Record(VkCommandBuffer cmdBuffer)
    {
        
    };
    
};

}//NS