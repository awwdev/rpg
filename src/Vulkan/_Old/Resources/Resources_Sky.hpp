//https://github.com/awwdev

#pragma once

#include "Vulkan/Context.hpp"
#include "Resources/CpuResources.hpp"
#include "Vulkan/_Old/Resources/Resources_Default.hpp"

#include "Vulkan/_Old/Factories/Sky/Sky_Pipeline.hpp"
#include "Vulkan/_Old/Factories/Sky/Sky_Shader.hpp" 

namespace rpg::vuk {

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