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

    void Record()
    {

    };

};

}//NS