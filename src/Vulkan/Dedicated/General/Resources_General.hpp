//https://github.com/awwdev

#pragma once
#include "Vulkan\Dedicated\General\General_RenderPass.hpp"
#include "Vulkan\Dedicated\General\General_Pipeline.hpp"
#include "Vulkan\Dedicated\General\General_Shader.hpp"
#include "Vulkan\Dedicated\General\General_VBO.hpp"
#include "Vulkan\Dedicated\General\General_UBO.hpp"

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