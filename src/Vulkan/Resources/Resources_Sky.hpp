//https://github.com/awwdev

#pragma once
#include "Vulkan/Core.hpp"
#include "Vulkan/Context.hpp"
#include "Resources/HostResources.hpp"
#include "Vulkan/Resources/Resources_Default.hpp"

#include "Vulkan/Factories/Sky/Sky_Pipeline.hpp"
#include "Vulkan/Factories/Sky/Sky_Shader.hpp" 

namespace rpg::vk {

struct Resources_Sky
{
    Shader            shader;
    Pipeline          pipeline;
    Sky_PushConstants pushConsts;


    void Create(VkCommandPool&, Resources_Common& common)
    {
        Sky_CreateShader        (shader);
        Sky_CreatePipeline      (pipeline, shader, common.renderPass);
    }

    void Recreate(VkCommandPool&, Resources_Common& common)
    {
        shader.~Shader();
        pipeline.~Pipeline();

        Sky_CreateShader        (shader);
        Sky_CreatePipeline      (pipeline, shader, common.renderPass);
    }
};

}//NS