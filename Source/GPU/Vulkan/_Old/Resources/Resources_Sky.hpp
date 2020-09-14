//https://github.com/awwdev

#pragma once

#include "GPU/Vulkan/Meta/Context.hpp"
#include "Resources/CpuResources.hpp"
#include "GPU/Vulkan/_Old/Resources/Resources_Default.hpp"

#include "GPU/Vulkan/_Old/Factories/Sky/Sky_Pipeline.hpp"
#include "GPU/Vulkan/_Old/Factories/Sky/Sky_Shader.hpp" 

namespace rpg::gpu::vuk {

struct Resources_Sky
{
    Shader            shader;
    Pipeline          pipeline;
    Sky_PushConstants pushConsts;


    void Create(VkCommandPool&, Resources_Common& common)
    {
        Sky_CreateShader        (shader);
        //Sky_CreatePipeline      (pipeline, shader, common.renderPass);
    }

    void Recreate(VkCommandPool&, Resources_Common& common)
    {
        shader.Clear();
        pipeline.Clear();

        Sky_CreateShader        (shader);
        //Sky_CreatePipeline      (pipeline, shader, common.renderPass);
    }
};

}//NS