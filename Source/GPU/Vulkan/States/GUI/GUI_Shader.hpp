//https://github.com/awwdev

#pragma once
#include "GPU/Vulkan/Meta/Context.hpp"
#include "GPU/Vulkan/Helper/Initializers.hpp"
#include "GPU/Vulkan/Helper/Utils.hpp"

namespace rpg::gpu::vuk {

struct GUI_Shader
{
    VkShaderModule modules [2];
    VkPipelineShaderStageCreateInfo info [2];

    void Create()
    {
        CreateShaderModule(
            "AssetsApp/Shaders/GUI/spv/gui.vert.spv", 
            VK_SHADER_STAGE_VERTEX_BIT,
            modules[0], info[0]);

        CreateShaderModule(
            "AssetsApp/Shaders/GUI/spv/gui.frag.spv", 
            VK_SHADER_STAGE_FRAGMENT_BIT,
            modules[1], info[1]);
    }

    void Clear()
    {
        FOR_CARRAY(modules, i)
            vkDestroyShaderModule(g_contextPtr->device, modules[i], nullptr);
        info[0] = info[1] = {};
    }

    ~GUI_Shader()
    {
        Clear();
    }
};

}//ns