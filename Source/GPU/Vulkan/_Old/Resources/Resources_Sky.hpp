//https://github.com/awwdev

#pragma once

#include "gpu/Vulkan/Meta/Context.hpp"
#include "res/_Old/CpuResources.hpp"
#include "gpu/Vulkan/_Old/res/Resources_Default.hpp"

#include "gpu/Vulkan/_Old/Factories/Sky/Sky_Pipeline.hpp"
#include "gpu/Vulkan/_Old/Factories/Sky/Sky_Shader.hpp" 

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