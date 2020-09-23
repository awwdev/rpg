//https://github.com/awwdev

#pragma once

#include "gpu/Vulkan/Meta/Context.hpp"
#include "gpu/Vulkan/_Old/Factories/Test/Test_RenderPass.hpp"
#include "gpu/Vulkan/_Old/Factories/Test/Test_Pipeline.hpp"
#include "gpu/Vulkan/_Old/Factories/Test/Test_Shader.hpp"

namespace rpg::gpu::vuk {

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