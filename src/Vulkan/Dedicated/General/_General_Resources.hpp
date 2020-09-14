//https://github.com/awwdev

#pragma once
#include "Vulkan\Dedicated\General\General_RenderPass.hpp"
#include "Vulkan\Dedicated\General\General_Pipeline.hpp"
#include "Vulkan\Dedicated\General\General_Shader.hpp"
#include "Vulkan\Dedicated\General\General_VertexBuffer.hpp"
#include "Vulkan\Dedicated\General\General_UniformBuffer.hpp"

namespace rpg::vuk {

struct Resources_General
{
    General_RenderPass renderPass;

    void Create(VkCommandPool cmdPool)
    {
        renderPass.Create(cmdPool);
    }
};

}//NS