//https://github.com/awwdev

#pragma once
#include "gpu/Vulkan/Context.hpp"

namespace rpg::gpu::vuk {

struct Line_Shader
{
    VkShaderModule modules [2];
    VkPipelineShaderStageCreateInfo stageInfo [2];

    void Create()
    {
        CreateShaderModule(
            "res/Shaders/Main/Instances/spv/line.vert.spv", 
            VK_SHADER_STAGE_VERTEX_BIT,
            modules[0], stageInfo[0]);

        CreateShaderModule(
            "res/Shaders/Main/Instances/spv/line.frag.spv", 
            VK_SHADER_STAGE_FRAGMENT_BIT,
            modules[1], stageInfo[1]);
    }

    void Destroy()
    {
        FOR_C_ARRAY(modules, i)
            vkDestroyShaderModule(g_contextPtr->device, modules[i], nullptr);
        stageInfo[0] = stageInfo[1] = {};
    }

    ~Line_Shader()
    {
        Destroy();
    }
};

}//ns