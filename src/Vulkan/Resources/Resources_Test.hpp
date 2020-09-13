//https://github.com/awwdev

#pragma once
#include "Vulkan/Core.hpp"
#include "Vulkan/Context.hpp"
#include "Vulkan/Factories/Test/Test_RenderPass.hpp"
#include "Vulkan/Factories/Test/Test_Pipeline.hpp"
#include "Vulkan/Factories/Test/Test_Shader.hpp"

namespace rpg::vk {

struct Resources_Test
{
    RenderPassTest   renderPass;
    Shader           shader;
    Pipeline         pipeline;

    void Create(VkCommandPool cmdPool)
    {
        Test_CreateShader           (shader);
        Test_CreateRenderPass       (renderPass, cmdPool);
        Test_CreatePipeline         (pipeline, shader, renderPass);
    }

    void Recreate(VkCommandPool cmdPool)
    {
        renderPass.Clear();
        pipeline.Clear();
        shader.Clear();
        Test_CreateShader           (shader);
        Test_CreateRenderPass       (renderPass, cmdPool);
        Test_CreatePipeline         (pipeline, shader, renderPass);
    }
};

}//NS